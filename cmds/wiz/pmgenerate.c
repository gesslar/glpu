/**
 * @file /Untitled-1
 * @description Generate an area map with Perlin noise
 *
 * @created 2024-07-24 - Gesslar
 * @last_modified 2024-07-24 - Gesslar
 *
 * @history
 * 2024-07-24 - Gesslar - Created
 */

// File: /cmds/wiz/pmgenerate.c

#include <daemons.h>

inherit STD_CMD;
inherit M_LOG;

// Function declarations
private string generate_area(string area_name, string area_type, int width, int height);
private string generate_cell(string area_type, float noise_value, float elevation_value);
private string generate_terrain(string area_type, float noise_value);
private int *generate_mask(int width, int height) ;
private int count_neighbors(int *mask, int x, int y, int width, int height) ;

private nosave int MAX_WIDTH = 100;
private nosave int MAX_HEIGHT = 100;

private nosave string *valid_area_types = ({ "forest", "mountain", "desert" });

mixed main(object tp, string str) {
    string aname, atype;
    int width, height;
    string result;
    string path;

    if(!str)
        return "Syntax: pmgenerate <area name> <area type> <width> <height>\n";

    if(sscanf(str, "%s %s %d %d", aname, atype, width, height) != 4)
        return "Syntax: pmgenerate <area name> <area type> <width> <height>\n";

    if(member_array(atype, valid_area_types) == -1)
        return "Invalid area type. Valid types are: " + implode(valid_area_types, ", ") + "\n";

    if(width <= 0 || height <= 0 || width > MAX_WIDTH || height > MAX_HEIGHT)
        return sprintf("Invalid dimensions. Width and height must be between 1 and %d.\n", MAX_WIDTH);

    _info(tp, "Generating area: " + aname + " of type: " + atype + " with dimensions: " + width + "x" + height);

    result = generate_area(aname, atype, width, height);

    path = home_path(tp) + aname + ".map";

    write_file(path, result, 1);

    return "Perlin noise-based area map file generated: " + path + "\n";
}

private string generate_area(string area_name, string area_type, int width, int height) {
    string *map;
    int y, x;
    string line;
    string cell;
    float noise_value, elevation_value;
    int *mask;

    map = allocate(height);
    mask = generate_mask(width, height);
    for (y = 0; y < height; y++) {
        line = "";
        for (x = 0; x < width; x++) {
            if (mask[y * width + x] == 0) {
                line += "XXX ";  // Impassable terrain with space
            } else {
                noise_value = PERLIN_D->perlin(x / 15.0, y / 15.0, 0.0);  // Adjusted scale
                elevation_value = PERLIN_D->perlin(x / 7.0, y / 7.0, 1.0);  // Adjusted scale
                cell = generate_cell(area_type, noise_value, elevation_value);
                line += cell;  // Cell already includes the space
            }
        }
        map[y] = line[0..<2];  // Remove trailing space
    }

    return implode(map, "\n");
}

private string generate_cell(string area_type, float noise_value, float elevation_value) {
    string terrain, elevation, feature;

    terrain = generate_terrain(area_type, noise_value);
    elevation = sprintf("%d", to_int((elevation_value + 1.0) * 1.5) % 4);  // 0-3 elevation
    feature = " ";  // Placeholder for future feature implementation

    return terrain + elevation + feature + " ";  // Three characters plus a space
}

private string generate_terrain(string area_type, float noise_value) {
    switch(area_type) {
        case "forest":
            return (noise_value < 0.9) ? "F" : "C"; // 90% forest, 10% clearing
        case "mountain":
            return (noise_value < 0.8) ? "M" : "H"; // 80% mountain, 20% hill
        case "desert":
            return (noise_value < 0.95) ? "S" : "O"; // 95% sand, 5% oasis
        default:
            return "P"; // Plain as default
    }
}

private int *generate_mask(int width, int height) {
    int *mask;
    int x, y;
    float noise_value;
    float center_x, center_y;
    float dx, dy, distance;

    mask = allocate(width * height);
    center_x = to_float(width) / 2.0;
    center_y = to_float(height) / 2.0;

    for(y = 0; y < height; y++) {
        for(x = 0; x < width; x++) {
            // Create an irregular shape using Perlin noise
            noise_value = PERLIN_D->perlin(to_float(x) / 15.0, to_float(y) / 15.0, 2.0);

            // Calculate distance from center
            dx = (to_float(x) - center_x) / center_x;
            dy = (to_float(y) - center_y) / center_y;
            distance = sqrt(dx*dx + dy*dy);

            // Determine if the point is inside the irregular shape
            if (noise_value > 0.3 || distance < 0.8) {
                mask[y * width + x] = 1;  // Inside the area
            } else {
                mask[y * width + x] = 0;  // Outside the area (impassable)
            }
        }
    }

    return mask;
}

private int count_neighbors(int *mask, int x, int y, int width, int height) {
    int count = 0;
    int dx, dy, nx, ny;
    for(dy = -1; dy <= 1; dy++) {
        for(dx = -1; dx <= 1; dx++) {
            if(dx == 0 && dy == 0) continue;
            nx = x + dx;
            ny = y + dy;
            if(nx >= 0 && nx < width && ny >= 0 && ny < height)
                count += mask[ny * width + nx];
        }
    }
    return count;
}

/**
 * @file /Untitled-1
 * @description Generate an area map with Perlin noise
 *
 * @created 2024/07/24 - Gesslar
 * @last_modified 2024/07/24 - Gesslar
 *
 * @history
 * 2024/07/24 - Gesslar - Created
 */

// File: /cmds/wiz/pmgenerate.c

#include <daemons.h>

inherit STD_CMD;
inherit M_LOG;

// Function declarations
private string generate_area(string area_name, string area_type, int width, int height);
private string generate_cell(string area_type, float noise_value, float elevation_value);
private string generate_terrain(string area_type, float noise_value);

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

    path = user_path(tp) + aname + ".map";

    write_file(path, result, 1);

    return "Perlin noise-based area map file generated: " + path + "\n";
}

private string generate_area(string area_name, string area_type, int width, int height) {
    string *map;
    int y, x;
    string line;
    string cell;
    float noise_value, elevation_value;

    map = allocate(height);

    for(y = 0; y < height; y++) {
        line = "";
        for(x = 0; x < width; x++) {
            noise_value = PERLIN_D->perlin(x / 10.0, y / 10.0, 0.0);
            elevation_value = PERLIN_D->perlin(x / 5.0, y / 5.0, 1.0);
            cell = generate_cell(area_type, noise_value, elevation_value);
            line += cell + " ";
        }
        map[y] = line[0..<2];  // Remove trailing space
    }

    return implode(map, "\n");
}

private string generate_cell(string area_type, float noise_value, float elevation_value) {
    string terrain, elevation;

    terrain = generate_terrain(area_type, noise_value);
    elevation = sprintf("%d", to_int(elevation_value * 4));  // 0-4 elevation

    return terrain + elevation + " ";  // Note the blank space for the feature column
}

private string generate_terrain(string area_type, float noise_value) {
    switch(area_type) {
        case "forest":
            return (noise_value < 0.8) ? "F" : "C"; // 80% forest, 20% clearing
        case "mountain":
            return (noise_value < 0.7) ? "M" : "H"; // 70% mountain, 30% hill
        case "desert":
            return (noise_value < 0.9) ? "S" : "O"; // 90% sand, 10% oasis
        default:
            return "P"; // Plain as default
    }
}

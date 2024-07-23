// File: /adm/daemons/area_generator.c

inherit STD_DAEMON;
inherit M_LOG;

#define MAX_WIDTH 50
#define MAX_HEIGHT 50

// Function prototypes
private string generate_cell(string area_type, int x, int y, int width, int ref *elevation_map);
private string generate_terrain(string area_type, int seed);
private void generate_elevation_map(int width, int height, int seed, int ref *elevation_map);
private void smooth_elevation_map(int width, int height, int ref *elevation_map);
private void save_area_file(string area_name, string *map);

void generate_area(string area_name, string area_type, int width, int height) {
    string *map;
    int y, x, seed;
    string line;
    string cell;
    int *elevation_map;

    if (width > MAX_WIDTH || height > MAX_HEIGHT) {
        error("Area dimensions exceed maximum allowed size.");
    }

    map = allocate(height);
    seed = time();
    elevation_map = allocate(height * width);

    generate_elevation_map(width, height, seed, ref elevation_map);
    smooth_elevation_map(width, height, ref elevation_map);

    for (y = 0; y < height; y++) {
        line = "";
        for (x = 0; x < width; x++) {
            cell = generate_cell(area_type, x, y, width, ref elevation_map);
            line += cell + " ";
        }
        map[y] = line[0..<2];  // Remove trailing space
    }

    save_area_file(area_name, map);
}

private string generate_cell(string area_type, int x, int y, int width, int ref *elevation_map) {
    string terrain, elevation;
    int *result;

    result = random_number(time_ns(), MAX_INT);
    terrain = generate_terrain(area_type, result[0]);

    elevation = sprintf("%d", elevation_map[y * width + x]);

    return terrain + elevation + " ";  // Note the blank space for the feature column
}

private string generate_terrain(string area_type, int seed) {
    int *result;
    int roll;

    result = random_number(seed, 100);
    roll = result[1];

    _log("Seed for area type: " + seed);  // Add this line for debugging
    _log("Area type: " + area_type + ", Roll: " + roll);  // Add this line for debugging

    switch(area_type) {
        case "forest":
            return (roll < 80) ? "F" : "C"; // 80% forest, 20% clearing
        case "mountain":
            return (roll < 70) ? "M" : "H"; // 70% mountain, 30% hill
        case "desert":
            return (roll < 90) ? "S" : "O"; // 90% sand, 10% oasis
        default:
            return "P"; // Plain as default
    }
}

private void generate_elevation_map(int width, int height, int seed, int ref *elevation_map) {
    int y, x;
    int *result;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            result = random_range(seed, 0, 4);
            seed = result[0];
            elevation_map[y * width + x] = result[1];
        }
    }
}

private void smooth_elevation_map(int width, int height, int ref *elevation_map) {
    int y, x, ny, nx, sum, count;
    int *new_map;

    new_map = allocate(height * width);
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            sum = elevation_map[y * width + x];
            count = 1;
            for (ny = y-1; ny <= y+1; ny++) {
                for (nx = x-1; nx <= x+1; nx++) {
                    if (ny >= 0 && ny < height && nx >= 0 && nx < width && (ny != y || nx != x)) {
                        if (abs(elevation_map[ny * width + nx] - elevation_map[y * width + x]) <= 1) {
                            sum += elevation_map[ny * width + nx];
                            count++;
                        } else {
                            sum += elevation_map[y * width + x] + (elevation_map[ny * width + nx] > elevation_map[y * width + x] ? 1 : -1);
                            count++;
                        }
                    }
                }
            }
            new_map[y * width + x] = sum / count;
        }
    }
    elevation_map = new_map;
}

private void save_area_file(string area_name, string *map) {
    string file_path, content;

    file_path = "/data/areas/" + area_name + ".map";
    content = implode(map, "\n");
    write_file(file_path, content, 1);  // The '1' flag ensures we overwrite the file
    _log("Area map generated and saved: " + file_path);
}

/**
 * @file /std/daemon/virtual_map.c
 * @description Daemon responsible for managing virtual maps for a zone.
 *
 * @created 2024-08-23 - Gesslar
 * @last_modified 2024-08-23 - Gesslar
 *
 * @history
 * 2024-08-23 - Gesslar - Created
 */

inherit STD_DAEMON;

// Forward declarations
public void set_map_file(string file);
public void load_map();
public int is_valid_room(int x, int y, int z);
public int get_map_width();
public int get_map_height();
public string *get_directions();
public mapping get_room_info(int x, int y, int z);

private nosave string map_file;
private nosave string *map_lines;
private nosave int map_width;
private nosave int map_height;

private nosave string *directions = ({"north", "northeast", "east", "southeast", "south", "southwest", "west", "northwest"});
private nosave int *dx = ({0, 1, 1, 1, 0, -1, -1, -1});
private nosave int *dy = ({-1, -1, 0, 1, 1, 1, 0, -1});
private nosave int *exit_symbols = ({'|', '/', '-', '\\', '|', '/', '-', '\\'});

public void set_map_file(string file) {
    map_file = file;
    load_map();
}

private void load_map() {
    if (!map_file) {
        error("No map file set. Use set_map_file() first.");
    }
    map_lines = explode(read_file(map_file), "\n");
    map_height = sizeof(map_lines);
    map_width = max(map(map_lines, (: strlen :)));
}

protected mapping get_exits(int x, int y, int z) {
    mapping exits = ([]);
    int i, nx, ny;

    for(i = 0; i < sizeof(directions); i++) {
        nx = x * 2 + dx[i];
        ny = y * 2 + dy[i];

        if(ny >= 0 && ny < map_height && nx >= 0 && nx < map_width) {
            if(sizeof(map_lines[ny]) > nx && map_lines[ny][nx] == exit_symbols[i]) {
                exits[directions[i]] = sprintf("%d,%d,%d", x + dx[i], y + dy[i], z);
            }
        }
    }

    return exits;
}

protected string get_room_type(int x, int y) {
    if(y * 2 < map_height && x * 2 < map_width) {
        return map_lines[y * 2][x * 2..x * 2];
    }
    return "";
}

public int is_valid_room(int x, int y, int z) {
    string room_type = get_room_type(x, y);
    return (room_type != "" && room_type != " ");
}

public int get_map_width() {
    return map_width / 2;
}

public int get_map_height() {
    return map_height / 2;
}

public string *get_directions() {
    return directions;
}

public mapping get_room_info(int x, int y, int z) {
    return ([
        "type": get_room_type(x, y),
        "exits": get_exits(x, y, z),
    ]);
}

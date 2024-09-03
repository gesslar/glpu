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
public void set_map_file(string file) ;
public void load_map() ;
public int is_valid_room(int x, int y, int z) ;
public int get_map_width() ;
public int get_map_height() ;
public string *get_directions() ;
public mapping get_room_info(int x, int y, int z) ;

private nosave string map_file ;
private nosave string *map_lines ;
private nosave mixed *noise_map_data ;
private nosave int map_width ;
private nosave int map_height ;
private nosave function noise_map ;
private nosave mixed max_noise_value ;
private nosave mixed min_noise_value ;

private nosave string *directions = ({"north", "northeast", "east", "southeast", "south", "southwest", "west", "northwest"}) ;
private nosave int *dx = ({0, 1, 1, 1, 0, -1, -1, -1}) ;
private nosave int *dy = ({-1, -1, 0, 1, 1, 1, 0, -1}) ;
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

protected void set_noise_map(function f) {
    mixed result ;

    if(!valid_function(f))
        error("Invalid noise map function.");

    noise_map = f;
    result = (*f)() ;

    if(!result || !pointerp(result))
        error("Invalid noise map function.");

    noise_map_data = result ;

    map_height = sizeof(noise_map_data);
    map_width = max(map(noise_map_data, (: sizeof :)));
}

protected void set_noise_map_range(mixed min, mixed max) {
    min_noise_value = min;
    max_noise_value = max;
}

protected void update_min_max(mixed value) {
    mixed min, max ;

    if(typeof(value) == "float") {
        min = nullp(min_noise_value) ?  MAX_FLOAT : min_noise_value ;
        max = nullp(max_noise_value) ? -MAX_FLOAT : max_noise_value ;
    } else {
        min = nullp(min_noise_value) ?  MAX_INT : min_noise_value ;
        max = nullp(max_noise_value) ? -MAX_INT : max_noise_value ;
    }

    if(value < min)
        min_noise_value = value;
    if(value > max)
        max_noise_value = value;
}

public mixed *get_noise_map_range() {
    return ({min_noise_value, max_noise_value});
}

mixed *query_noise_map_data() {
    return noise_map_data;
}

protected mapping get_exits(int x, int y, int z) {
    mapping exits = ([]);
    int i, nx, ny;

    if(!noise_map_data) {
        for(i = 0; i < sizeof(directions); i++) {
            nx = x * 2 + dx[i];
            ny = y * 2 + dy[i];

            if(ny >= 0 && ny < map_height && nx >= 0 && nx < map_width) {
                if(sizeof(map_lines[ny]) > nx && map_lines[ny][nx] == exit_symbols[i]) {
                    exits[directions[i]] = sprintf("%d,%d,%d", x + dx[i], y + dy[i], z);
                }
            }
        }
    } else {
        for(i = 0; i < sizeof(directions); i++) {
            if(y >= 0 && y < map_height && x >= 0 && x < map_width) {
                if(noise_map_data[y][x] > 0.5) {
                    exits[directions[i]] = sprintf("%d,%d,%d", x + dx[i], y + dy[i], z);
                }
            }
        }
    }
    return exits;
}

/* protected */ mixed get_room_type(int x, int y) {
    if(!noise_map_data) {
        if(y * 2 < map_height && x * 2 < map_width) {
            return map_lines[y * 2][x * 2..x * 2];
        }
    } else {
        if(y >= 0 && y < map_height && x >= 0 && x < map_width) {
            return noise_map_data[y][x];
        }
    }

    printf("x is %d, y is %d\n", x, y);
    printf("map width is %d, map height is %d\n", map_width, map_height);

    return "";
}

public int is_valid_room(int x, int y, int z) {
    mixed room_type ;

    if(!noise_map_data)
        room_type = get_room_type(x, y);
    else
        room_type = noise_map_data[y][x];

    return (room_type != "" && room_type != " ");
}

public int get_map_width() {
    if(noise_map_data)
        return map_width;
    return map_width / 2;
}

public int get_map_height() {
    if(noise_map_data)
        return map_height;
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

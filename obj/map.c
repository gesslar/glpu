/* map.c

 Tacitus @ LPUniversity
 24-Aug-06
 An object that represents a map

*/

/* INTRODUCTION
 *
 * This object represents a map that is
 * parses from a file. Each line is a row (y)
 * in the map and each column (x) is also a
 * column in the map.
 *
 * This object provides a few simple accesor functions
 * to get information from the map and to minipulate the
 * map.
 *
 * This should load on any MudOS mud and most likely ldmud,
 * dgd, and others with ease.
 *
 */

/* Global Variables */

mixed *map;         /* 2D array containing map tiles  */
string map_source;   /* Map Source file (aka filename) */

/* Function Prototypes */

void clear();
private int parse_map(string filename);

/* Functions */
void setup() {
    clear();
}


/* Clears the map of all data */
void clear() {
    map = ({});
    map_source = "";
}

/* Load a map from file
 *
 * Will return 0 if file does not exist.
 * See comments on parse_map for further
 * error codes.
 *
 */

int load(string filename) {
    if(!file_exists(filename)) return 0;
    else {
        clear();
        return parse_map(filename);
    }
}

/* Returns 1 if a map is loaded or 0 if false */
int is_loaded() {
    if(sizeof(map) > 0) return 1;
    else return 0;
}

/* Returns the filename of current map */
string get_map_source() {
    return map_source;
}

/* Parses the map and creates 2D array
 *
 * This function can only be called internally.
 *
 * Returns -2 when unable to read file.
 * Returns -3 when file is empty.
 * Returns 1 on success
 *
 */
private int parse_map(string filename) {
    string file_contents, *file_exploded;
    int y, x;

    file_contents = read_file(filename);
    if(!file_contents || !stringp(file_contents))
    return -2;

    file_exploded = explode(file_contents, "\n");
    if(!pointerp(file_exploded) || sizeof(file_exploded) <= 0)
    return -3;

    map = allocate(sizeof(file_exploded));
    map_source = filename;

    for(y = 0; y < sizeof(file_exploded); y++) {
        map[y] = allocate(strlen(file_exploded[y]));

        for(x = 0; x < sizeof(map[y]); x++) {
            map[y][x] = file_exploded[y][x..x];
        }
    }

    return 1;
}

/* Get the width of a row or if no argument is provided
 * then the row with the greatest width is returned.
 * -1 is returned when you provide an invalid argument.
 */
varargs int get_width(int y) {
    if(intp(y)) {
        if(y >= sizeof(map) || y < 0) return -1;
        else return sizeof(map[y]);
    } else {
        int max_width = 0;

        foreach(mixed element in map)
        if(sizeof(element) > max_width) max_width = sizeof(element);

        return max_width;
    }
}

/* Returns the height of the map */
int get_height() {
    return sizeof(map);
}

/* Returns the tile type at the coordinates provided
 * Will return -1 for invalid or not existing coordinates
 */
mixed get_tile_at(int x, int y) {
    if(x < 0 || y < 0) return -1;
    if(y >= sizeof(map) || sizeof(map[y]) <= x) return -1;
    return "" + map[y][x];
}

/* Allows you to set the tile type at the coordinates provided
 * Returns 1 for success, 0 for failure, and -1 for invalid argument */
int set_tile_at(mixed tile_type, int x, int y) {
    if(x < 0 || y < 0) return -1;
    if(y >= sizeof(map) || sizeof(map[y]) <= x) return 0;
    map[y][x] = tile_type;
    return 1;
}

/* Returns 1 if tile type at coordinates match the tiletype
 * passed as the first argument. Returns 0 if false.
 * Returns -1 for invalid argument.
 */
int is_tile_type(mixed tile_type, int x, int y) {
    if(x < 0 || y < 0) return -1;
    if(y >= sizeof(map) || sizeof(map[y]) <= x) return -1;
    if(map[y][x] == tile_type) return 1;
    else return 0;
}

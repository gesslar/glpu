inherit STD_VIRTUAL_MAP;

// Forward declarations
private void setup_cavern_shorts();
private void setup_cavern_longs();
private mixed *generate_map();
private int surrounding_cells_are_walls(int z, int y, int x, int px, int py) ;
private int any_surrounding_cell_is_a_path(int z, int y, int x) ;
private void generate_layer(int z);
private void connect_layers() ;
private mixed *find_layer_connections(int z) ;
private int count_neighbour_walls(int z, int y, int x);
private string *shuffle_directions(int z, int y, int x);
private void setup_dimensions();
private string set_path(int z, int y, int x) ;
private string clear_path(int z, int y, int x) ;
public int is_path(int z, int y, int x) ;
private string clear_wall(int z, int y, int x) ;
private string set_wall(int z, int y, int x) ;
public int is_wall(int z, int y, int x) ;
private string clear_down(int z, int y, int x) ;
private string set_down(int z, int y, int x) ;
public int is_down(int z, int y, int x) ;
private string clear_up(int z, int y, int x) ;
private string set_up(int z, int y, int x) ;
public int is_up(int z, int y, int x) ;
varargs string determine_room_type(int z, int y, int x) ;
private int oob(int z, int y, int x) ;
private void determine_exit_and_entrance() ;
int *find_a_path(int z) ;
int *find_a_wall(int z) ;

// Constants
private nosave int DEPTH = 0, HEIGHT = 1, WIDTH = 2;
private nosave int MIN_Z, MAX_Z, MIN_Y, MAX_Y, MIN_X, MAX_X;
private nosave int WALL = 1, PATH = 2, UP = 3, DOWN = 4;
private nosave int MIN_PATH = 2 ;

// The directions for the maze. Used to calculate the coordinates of the
// neighbouring cells.
private nosave mapping DIRECTIONS = ([
  "north"    : (["dz":  0, "dy": -1, "dx":  0]),
  "south"    : (["dz":  0, "dy":  1, "dx":  0]),
  "east"     : (["dz":  0, "dy":  0, "dx":  1]),
  "west"     : (["dz":  0, "dy":  0, "dx": -1]),
  "up"       : (["dz":  1, "dy":  0, "dx":  0]),
  "down"     : (["dz": -1, "dy":  0, "dx":  0]),
]) ;

private nosave int *entrance, *exit;

// Variables
private nosave mapping short_descriptions;
private nosave mapping long_descriptions;

private nosave int *dimensions;
// Dimension configuration {min, range} for z, y, x
private nosave mixed *dimension_config = ({ ({ 5, 1 }), ({ 25, 10 }), ({ 25, 10 }) });
private nosave float coverage_percentage = 0.03 ; // 5%
private nosave int min_room_size = 1 ;
private nosave int max_room_size = 3 ;
private mixed *neighbour_excludes = ({}) ;
private mixed *all_paths, *all_walls;
private nosave mixed *all_connections ;
private nosave mixed seed = 42;

// The cavern map
private nosave mixed *cavern_map;

void setup() {
  set_no_clean(1) ;

  setup_dimensions();

  rm("/log/cavern");

  // Apply the map generator
  apply_map_generator((: generate_map :));

  // Setup descriptions
  setup_cavern_shorts();
  setup_cavern_longs();

  // Determine the exit and entrance
  determine_exit_and_entrance();
}

/**
 * Setup the dimensions of the cavern. This sets depth, height, and width.
 */
private void setup_dimensions() {
  mixed *result;

  dimensions = allocate(3);

  // Depth
  result = prandom(seed, dimension_config[DEPTH][1]);
  seed = result[0];
  dimensions[DEPTH] = result[1] + dimension_config[DEPTH][0];
  // Initialize MIN_Z and MAX_Z if necessary

  // Height
  result = prandom(seed, dimension_config[HEIGHT][1]);
  seed = result[0];
  dimensions[HEIGHT] = result[1] + dimension_config[HEIGHT][0];
  // Initialize MIN_Y and MAX_Y if necessary

  // Width
  result = prandom(seed, dimension_config[WIDTH][1]);
  seed = result[0];
  dimensions[WIDTH] = result[1] + dimension_config[WIDTH][0];
  // Initialize MIN_X and MAX_X if necessary

  MIN_Z = 0;
  MAX_Z = dimensions[DEPTH] - 1;
  MIN_Y = 0;
  MAX_Y = dimensions[HEIGHT] - 1;
  MIN_X = 0;
  MAX_X = dimensions[WIDTH] - 1;
}

/**
 * Generate the map using simplex2 for tunnels.
 */
protected mixed *generate_map() {
  mixed *result;
  int x, y, z ;

  cavern_map = allocate(dimensions[DEPTH]);
  all_paths = allocate(dimensions[DEPTH]);
  all_walls = allocate(dimensions[DEPTH]);

  for(z = 0; z < dimensions[DEPTH]; z++) {
    cavern_map[z] = allocate(dimensions[HEIGHT]);
    all_paths[z] = ({});
    all_walls[z] = ({});
    for(y = 0; y < dimensions[HEIGHT]; y++) {
      cavern_map[z][y] = allocate(dimensions[WIDTH], "") ;
      for(x = 0; x < dimensions[WIDTH]; x++)
        set_wall(z, y, x) ;
    }
  }

  for(z = 0; z < dimensions[DEPTH]; z++)
    generate_layer(z) ;

  connect_layers();

  return cavern_map;
}

int query_target_tiles() {
  return to_int((dimensions[WIDTH] * dimensions[HEIGHT]) * coverage_percentage);
}

int query_tiles_carved() {
  int count = 0;
    for(int y = 0; y < dimensions[HEIGHT]; y++) {
      for(int x = 0; x < dimensions[WIDTH]; x++) {
        if(is_path(0, y, x)) {
          count++;
        }
      }
  }
  return count;
}

/**
 * Generate tunnels using cellular automata.
 */
private void generate_layer(int z) {
  int target_tiles = to_int((dimensions[WIDTH] * dimensions[HEIGHT]) * coverage_percentage);
  int carved_tiles = 0;
  int x, y, start_x, start_y;
  string dir, *dirs;
  mixed *result;
  mixed *stack ;

  dirs = keys(DIRECTIONS) - ({ "up", "down" });

  // Initialize the drunkard's starting position
  result = prandom(seed, dimensions[WIDTH]);
  seed = result[0];
  start_x = result[1];

  result = prandom(seed, dimensions[HEIGHT]);
  seed = result[0];
  start_y = result[1];

  x = start_x;
  y = start_y;

  // Stack to keep track of positions for backtracking
  stack = ({ ({ x, y }) });

  while (carved_tiles < target_tiles && sizeof(stack) > 0) {
    string *valid_dirs = ({});
    string *shuffled_dirs;

    reset_eval_cost() ;

    x = stack[<1][0];
    y = stack[<1][1];

    if (!is_path(z, y, x)) {
      set_path(z, y, x);
      carved_tiles++;
    }

    // Shuffle directions to introduce randomness
    result = pshuffle(seed, dirs);
    seed = result[0];
    shuffled_dirs = result[1];

    // Find valid directions
    foreach (dir in shuffled_dirs) {
      int nx = x + DIRECTIONS[dir]["dx"];
      int ny = y + DIRECTIONS[dir]["dy"];

      if (oob(z, ny, nx))
        continue;

      // Ensure the new cell and its neighbors are walls
      if (is_wall(z, ny, nx) && surrounding_cells_are_walls(z, ny, nx, x, y)) {
        valid_dirs += ({ dir });
      }
    }

    if (sizeof(valid_dirs) > 0) {
      int nx, ny;
      mixed *excludes = ({}) ;

      // Randomly select a valid direction
      dir = valid_dirs[0]; // Already shuffled, take the first
      nx = x + DIRECTIONS[dir]["dx"];
      ny = y + DIRECTIONS[dir]["dy"];

      // Move to the new cell
      stack += ({ ({ nx, ny }) });
    } else {
      int *new_start ;
      string *dirs_to_check = keys(DIRECTIONS) - ({ "up", "down" }) ;
      mixed *walls = copy(all_walls[z]) ;
      int xx = 0 ;

      // No valid moves, backtrack
      stack = stack[0..<2];

      while(sizeof(walls) > 0) {
        int count = 0 ;
        int *wall, i ;

        result = prandom(seed, sizeof(walls)) ;
        seed = result[0] ;
        i = result[1] ;
        wall = walls[i] ;
        walls = remove_array_element(walls, i) ;

        foreach(dir in dirs_to_check) {
          int nx = wall[1] + DIRECTIONS[dir]["dx"];
          int ny = wall[0] + DIRECTIONS[dir]["dy"];

          if(is_path(z, ny, nx))
            count++ ;
        }

        // destruct() ;

        // Record a success at x,y
        if(count == 1 || count == 2) {
          stack += ({ ({ wall[1], wall[0] }) });
          break ;
        }
      }
    }
  }
}

private int surrounding_cells_are_walls(int z, int y, int x, int px, int py) {
  // Define arrays for cardinal direction offsets
  int *dxs = ({ -1, 0, 1, 0 });
  int *dys = ({  0, 1, 0, -1 });

  for (int i = 0; i < 4; i++) {
    int nx = x + dxs[i];
    int ny = y + dys[i];

    if (nx == px && ny == py)
      continue; // Skip the parent cell

    if (oob(z, ny, nx))
      continue; // Ignore out-of-bounds cells

    if (!is_wall(z, ny, nx))
      return 0; // Adjacent cell is not a wall
  }
  return 1; // All orthogonal adjacent cells are walls
}

private mixed *any_surrounding_cell_is_a_path(int z, int y, int x) {
  // Define arrays for cardinal direction offsets
  int *dxs = ({ -1, 0, 1, 0 });
  int *dys = ({  0, 1, 0, -1 });
  mixed *result = ({});

  for (int i = 0; i < 4; i++) {
    int nx = x + dxs[i];
    int ny = y + dys[i];

    if (oob(z, ny, nx))
      continue; // Ignore out-of-bounds cells

    if (is_path(z, ny, nx))
      result += ({ ({ z, ny, nx }) });
  }

  if(sizeof(result) == 2)
    return result;
  else
    return ({});
}

// Connect the layers together. This will connect the layers together by
// setting the down and up bits for the current layer and the layer below in
// the connections returned by find_layer_connections.
private void connect_layers() {
  int z ;
  int i, sz ;
  // Go layer by layer from the top to the second from the bottom, connecting
  // random spots in the current layer to random spots in the layer below.

  all_connections = allocate(dimensions[DEPTH]) ;

  for(z = 0; z < dimensions[DEPTH]-1; z++) {
    mixed *connections ;

    connections = find_layer_connections(z) ;
    all_connections[z] = connections ;

    for(i = 0, sz = sizeof(connections); i < sz; i++) {
      mixed *connection = connections[i] ;

      set_down(connection[0][0], connection[0][1], connection[0][2]) ;
      set_up(connection[1][0], connection[1][1], connection[1][2]) ;
    }
  }
}

/*
// Finds all paths on the current layer that have a corresponding path set
// at the same x,y coordinates in the layer below. Returns an array of
// 1-3 random connections.
mixed *find_layer_connections(int z) {
  int *possible_down_exits ;
  int *possible_up_exits ;
  mixed *candidates ;
  mixed *result ;
  mixed *current = cavern_map[z] ;
  mixed *below = cavern_map[z+1] ;
  mixed *connections = ({});
  int num_connections ;
  int x, y ;

  candidates = ({});

  for(y = 0; y < dimensions[HEIGHT]; y++)
    for(x = 0; x < dimensions[WIDTH]; x++) {
      int *candidate = ({ z+1, y, x }) ;

      if(is_path(z, y, x) && is_path(candidate[0], candidate[1], candidate[2]))
        candidates += ({ ({ candidate[1..]... }) }) ;
    }

  if(sizeof(candidates) == 0)
    return null ;

  result = prandom(seed, 5) ;
  seed = result[0] ;
  num_connections = result[1] + 10 ;

  while(num_connections--) {
    result = pelement_of(seed, candidates) ;
    seed = result[0] ;
    connections += ({ result[1] }) ;
  }

  return connections ;
}
*/
mixed *find_layer_connections(int z) {
  int *path_on_this_floor = find_a_path(z) ;
  int *path_on_next_floor = find_a_path(z+1) ;

  return ({ ({ path_on_this_floor, path_on_next_floor }) }) ;
}

// Setup short descriptions for the cavern rooms
private void setup_cavern_shorts() {
  short_descriptions = ([
    "cavern" : ({
      "Silent Cavern",
      "Echoing Chamber",
      "Dark Hollow",
      "Misty Tunnel",
    }),
  ]);
}

// Setup long descriptions for the cavern rooms
private void setup_cavern_longs() {
  long_descriptions = ([
    "cavern" : ({
      "You find yourself in a silent cavern, the air thick with moisture. Stalactites hang from the ceiling, dripping water into puddles on the floor.",
      "This echoing chamber resonates with the faint sounds of your movements. The walls are rough and uneven, carved by centuries of natural forces.",
      "A dark hollow lies before you, shrouded in shadows. The faint outline of a passageway suggests the path forward.",
      "A misty tunnel stretches out, the air cool and damp. The light barely penetrates the gloom, making it difficult to see far ahead.",
    }),
  ]);
}

// Setup the short description for a room.
public void setup_short(object room, string file) {
  int *coords = room->get_virtual_coordinates();
  string room_type ;

  coords[0] = abs(coords[0]);
  room_type = determine_room_type(coords...);

  room->set_short(element_of(short_descriptions[room_type]) + " (Level "+(abs(coords[0])+1)+")");
}

// Setup the long description for a room.
public void setup_long(object room, string file) {
  int *coords = room->get_virtual_coordinates();
  string room_type ;

  coords[0] = abs(coords[0]);
  room_type = determine_room_type(coords...);

  room->set_long(element_of(long_descriptions[room_type]));
}

// Setup the exits for the cavern. This will setup the exits for the maze by
// adding the exits to the room.
public void setup_exits(object room) {
  int *coords = room->get_virtual_coordinates() ;
  int x, y, z, layer ;
  int *adjacent ;
  int max_x, max_y, max_z ;
  string file = query_file_name(room);
  string room_type ;
  string test ;
  mixed *layer_connections ;

  x = coords[2] ;
  y = coords[1] ;
  z = coords[0] ;

  max_x = get_map_width() ;
  max_y = get_map_height() ;
  max_z = get_map_depth() ;

  layer = abs(z) ;

  if(is_wall(layer, y, x))
    return ;

  room_type = cavern_map[layer][y][x] ;

  foreach(string dir, mapping info in DIRECTIONS) {
    if(dir == "up") {
      if(!is_up(layer, y, x))
        continue ;

      layer_connections = all_connections[layer-1] ;

      foreach(mixed *connection in layer_connections) {
        int *down = connection[0] ;
        int *up = connection[1] ;

        if(up[0] == layer && up[1] == y && up[2] == x) {
          room->add_exit(dir, sprintf("%d,%d,%d", down[2], down[1], down[0]*-1)) ;
          break ;
        }
      }
    } else if(dir == "down") {
      if(!is_down(layer, y, x))
        continue ;

      layer_connections = all_connections[layer] ;
      foreach(mixed *connection in layer_connections) {
        int *down = connection[0] ;
        int *up = connection[1] ;

        if(down[0] == layer && down[1] == y && down[2] == x) {
          room->add_exit(dir, sprintf("%d,%d,%d", up[2], up[1], up[0]*-1)) ;
          break ;
        }
      }
    } else {
      int nz = z, ny = y + DIRECTIONS[dir]["dy"], nx = x + DIRECTIONS[dir]["dx"] ;
      int lz = layer, ly = y + DIRECTIONS[dir]["dy"], lx = x + DIRECTIONS[dir]["dx"] ;

      if(!is_path(lz, ly, lx))
        continue ;

      room->add_exit(dir, sprintf("%d,%d,%d", nx, ny, nz)) ;
    }
  }

  if(z == exit[0] && y == exit[1] && x == exit[2])
    room->add_exit("up", sprintf("/d/wastes/%d,%d,%d", entrance[2], entrance[1], entrance[0])) ;
}

int *query_exit() {
  return exit;
}

int *query_entrance() {
  return entrance;
}

// Check if the coordinates are out of bounds.
public int oob(int z, int y, int x) {
  return z < MIN_Z || z > MAX_Z ||
         y < MIN_Y || y > MAX_Y ||
         x < MIN_X || x > MAX_X ;
}

// Clear a wall at the given coordinates.
private string clear_wall(int z, int y, int x) {
  string result;

  if(oob(z, y, x))
    return 0;

  result = cavern_map[z][y][x] = clear_bit(cavern_map[z][y][x], WALL);

  for(int i = 0; i < sizeof(all_walls[z]); i++) {
    if(all_walls[z][i][0] == y && all_walls[z][i][1] == x) {
      all_walls[z] = remove_array_element(all_walls[z], i);
      break;
    }
  }

  return result;
}

private string set_wall(int z, int y, int x) {
  string result;

  if(oob(z, y, x))
    return 0;

  clear_path(z, y, x);
  result = cavern_map[z][y][x] = set_bit(cavern_map[z][y][x], WALL);

  all_walls[z] += ({ ({ y, x }) });

  return result;
}

// Check if the coordinates are a wall.
public int is_wall(int z, int y, int x) {
  if(oob(z, y, x))
    return 0;

  return test_bit(cavern_map[z][y][x], WALL) ;
}

// Clear a path at the given coordinates.
private string clear_path(int z, int y, int x) {
  string result ;

  if(oob(z, y, x))
    return 0 ;

  result = cavern_map[z][y][x] = clear_bit(cavern_map[z][y][x], PATH) ;

  for(int i = 0; i < sizeof(all_paths[z]); i++) {
    if(all_paths[z][i][0] == y && all_paths[z][i][1] == x) {
      all_paths[z] = remove_array_element(all_paths[z], i);
      break;
    }
  }

  return result ;
}

private string set_path(int z, int y, int x) {
  string result ;

  if(oob(z, y, x))
    return 0 ;

  clear_wall(z, y, x) ;
  result = cavern_map[z][y][x] = set_bit(cavern_map[z][y][x], PATH) ;

  all_paths[z] += ({ ({ y, x }) });

  return result ;
}

// Check if the coordinates are a path.
public int is_path(int z, int y, int x) {
  if(oob(z, y, x))
    return 0 ;

  return test_bit(cavern_map[z][y][x], PATH) ;
}

// Clear a down connection at the given coordinates.
private string clear_down(int z, int y, int x) {
  string result ;

  if(oob(z, y, x))
    return 0 ;

  result = cavern_map[z][y][x] = clear_bit(cavern_map[z][y][x], DOWN) ;

  return result ;
}

// Set a down connection at the given coordinates.
private string set_down(int z, int y, int x) {
  string result ;

  if(oob(z, y, x))
    return 0 ;

  result = cavern_map[z][y][x] = set_bit(cavern_map[z][y][x], DOWN) ;

  return result ;
}

// Check if the coordinates are a down connection.
public int is_down(int z, int y, int x) {
  if(oob(z, y, x))
    return 0 ;

  return test_bit(cavern_map[z][y][x], DOWN) ;
}

// Clear an up connection at the given coordinates.
private string clear_up(int z, int y, int x) {
  string result ;

  if(oob(z, y, x))
    return 0 ;

  result = cavern_map[z][y][x] = clear_bit(cavern_map[z][y][x], UP) ;

  return result ;
}

// Set an up connection at the given coordinates.
private string set_up(int z, int y, int x) {
  string result ;

  if(oob(z, y, x))
    return 0 ;

  result = cavern_map[z][y][x] = set_bit(cavern_map[z][y][x], UP) ;

  return result ;
}

// Check if the coordinates are an up connection.
public int is_up(int z, int y, int x) {
  if(oob(z, y, x))
    return 0 ;

  return test_bit(cavern_map[z][y][x], UP) ;
}

// Determine the room type based on the noise value.
varargs string determine_room_type(int z, int y, int x) {
  if(is_path(z, y, x))
    return "cavern" ;
  else if(is_wall(z, y, x))
    return "wall" ;
}

private nosave string wastes_daemon = "/d/wastes/wastes_daemon" ;
// Determine the exits and entrances for the maze. This will determine the
// exits and entrances for the maze by finding a valid entrance from the
// forest and a valid entrance from the wastes.
void determine_exit_and_entrance() {
  mixed *result;
  int height, width ;

  // Set the exit from a random place in the cavern.
  exit = find_a_path(0);

  // Find a random place in the wastes to set the entrance.
  height = wastes_daemon->get_map_height() ;
  width = wastes_daemon->get_map_width() ;

  // Pick a random room in the wastes and set a down exit to the cavern
  while(true) {
    int y, x ;
    string room_type ;

    result = prandom(seed, height);
    seed = result[0];
    y = result[1];

    result = prandom(seed, width);
    seed = result[0];
    x = result[1];

    room_type = wastes_daemon->determine_room_type(0, y, x);

    if(room_type == "rocky") {
      entrance = ({ 0, y, x }) ;
      break ;
    }
  }
}

// Display the cavern.
void display_cavern() {
  int x, y, z ;
  string out = "" ;

  for(z = 0; z < dimensions[DEPTH]; z++) {
    out = "" ;
    for(y = 0; y < dimensions[HEIGHT]; y++) {
      for(x = 0; x < dimensions[WIDTH]; x++) {
        if(is_wall(z, y, x))
          out += " " ;
        else
          out += "#" ;
      }
      out += "\n" ;
    }
    printf("\n%s", out) ;
  }
}

// Display a layer of the cavern.
void display_layer(int z) {
  int x, y ;
  string out = "" ;
  mixed *layer = cavern_map[z] ;

  printf("\n") ;
  if(sizeof(layer) != dimensions[HEIGHT])
    error("Layer has the wrong height.") ;

  for(y = 0; y < dimensions[HEIGHT]; y++) {
    out = "" ;
    if(sizeof(layer[y]) != dimensions[WIDTH])
      error("Layer has the wrong width.") ;

    for(x = 0; x < dimensions[WIDTH]; x++) {
      if(is_wall(z, y, x))
        out += " " ;
      else
        out += "#" ;
    }
    printf("%s\n", out) ;
  }
}

mixed *find_all_paths(int z) {
  return all_paths[z];
}

int *find_a_path(int z) {
  mixed *result ;
  int i ;

  result = prandom(seed, sizeof(all_paths[z]));
  seed = result[0];
  i = result[1];

  return ({ z, all_paths[z][i]... }) ;
}

int *find_a_wall(int z) {
  mixed *result ;
  int i ;

  result = prandom(seed, sizeof(all_walls[z]));
  seed = result[0];
  i = result[1];

  return ({ z, all_walls[z][i]... }) ;
}

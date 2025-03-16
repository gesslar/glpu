inherit STD_VIRTUAL_MAP;

private void init_maze() ; // Added forward declaration
private void setup_dimensions();
private void setup_shorts();
private void setup_longs();
protected mixed *generate_map();
private void generate_layer(int z);
private void display_maze();
private void display_layer(int z);
private void connect_layers();
private mixed *find_layer_connections(int z);
private void determine_exits_and_entrances();
private void determine_centre_room(int z);
private int *query_centre_room(int z);
private mixed *query_centre_rooms();
private int oob(int z, int y, int x);
private string clear_wall(int z, int y, int x);
private string set_wall(int z, int y, int x);
public int is_wall(int z, int y, int x);
private string clear_path(int z, int y, int x);
private string set_path(int z, int y, int x);
public int is_path(int z, int y, int x);
private string clear_down(int z, int y, int x);
private string set_down(int z, int y, int x);
public int is_down(int z, int y, int x);
private string clear_up(int z, int y, int x);
private string set_up(int z, int y, int x);
public int is_up(int z, int y, int x);
private int *find_neighbour_path(int z, int y, int x, int source);
private void connect_to_neighbour_path(int *curr, mixed *neighbours);
public int get_room_count();

// The short descriptions for the maze.
private nosave string *short_descriptions;
// Default short description for the maze.
private nosave string default_short_description;

// The long descriptions for the maze.
private nosave string *long_descriptions;
// Default long description for the maze.
private nosave string default_long_description;

// Dimension configuration for the maze.
// The first element is the minimum size and is added to the result of the
// prandom function applied to the second element to get the actual size.
// It is in the order of z, y, x.
private nosave mixed *dimension_config = ({ ({ 25, 5 }), ({ 25, 5 }), ({ 25, 5 }),  });
private nosave int *dimensions;

// Some constants
private nosave int DEPTH = 0, HEIGHT = 1, WIDTH = 2;
private nosave int FOREST = 0, WASTES = 1;
private nosave int NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3;

// Exits out of the maze.
private nosave mixed *exits_out = allocate(2) ; // north, east
// External entrances to the maze.
private nosave mixed *external_entrances = allocate(2) ; // forest, wastes
// The centre rooms of the maze.
private nosave mixed *centre_rooms;
// The seed for the random number generator.
private nosave mixed seed = BOOT_D->query_boot_number();
// private nosave mixed seed = 42;
// The maze.
private nosave mixed *maze;

// Bitwise flags for the maze.
private nosave int WALL = (1<<1);
private nosave int PATH = (1<<2);
private nosave int UP   = (1<<3);
private nosave int DOWN = (1<<4);

// The number of rooms in the maze.
private nosave int room_count = 0;

// The minimum and maximum coordinates for the maze.
private nosave int MIN_X, MIN_Y, MAX_X, MAX_Y, MIN_Z, MAX_Z;
// The directions for the maze. Used to calculate the coordinates of the
// neighbouring cells.
private nosave mapping DIRECTIONS = ([
  "north" : (["dz":  0, "dy": -1, "dx":  0]),
  "south" : (["dz":  0, "dy":  1, "dx":  0]),
  "east"  : (["dz":  0, "dy":  0, "dx":  1]),
  "west"  : (["dz":  0, "dy":  0, "dx": -1]),
  "up"    : (["dz":  1, "dy":  0, "dx":  0]),
  "down"  : (["dz": -1, "dy":  0, "dx":  0]),
]);

void setup() {
  set_no_clean(1);

  if(dimensions)
    return;

  // Apply the map generator. This will generate the maze.
  apply_map_generator((: generate_map :));

  // Setup the short and long descriptions for use when the room is first
  //created.
  setup_shorts();
  setup_longs();
}

/**
 * Setup the dimensions of the maze. This will set the depth, height and width
 */
private void setup_dimensions() {
  mixed *result;

  dimensions = allocate(3);

  // Depth
  result = prandom(seed, dimension_config[DEPTH][1]);
  seed = result[0];
  dimensions[DEPTH] = result[1] + dimension_config[DEPTH][0];
  MIN_Z = 0;
  MAX_Z = dimensions[DEPTH] - 1;

  // Height
  result = prandom(seed, dimension_config[HEIGHT][1]);
  seed = result[0];
  dimensions[HEIGHT] = result[1] + dimension_config[HEIGHT][0];
  MIN_Y = 0;
  MAX_Y = dimensions[HEIGHT] - 1;

  // Width
  result = prandom(seed, dimension_config[WIDTH][1]);
  seed = result[0];
  dimensions[WIDTH] = result[1] + dimension_config[WIDTH][0];
  MIN_X = 0;
  MAX_X = dimensions[WIDTH] - 1;

  // Allocate the centre rooms array.
  centre_rooms = allocate(dimensions[DEPTH]);
}

/**
 * Generate the maze. This will generate the maze layer by layer. Once it has
 * generated the maze, it will connect the layers together and determine the
 * exits from and entrances to the maze from the forest and wastes.
 *
 * @return The maze.
 */
protected mixed *generate_map() {
  int z;

  setup_dimensions();
  init_maze();

  for(z = MIN_Z; z <= MAX_Z; z++) {
    generate_layer(z);
    determine_centre_room(z);
  }

  connect_layers();
  determine_exits_and_entrances();

  return maze;
}

/**
 * Generate a layer of the maze. This will generate a layer of the maze. It
 * uses a depth-first search algorithm to generate the maze.
 *
 * @param z The layer to generate.
 */
private void generate_layer(int z) {
  int start_x, start_y;
  mixed *stack = ({});
  mapping *directions = ({ });
  mixed *result;

  // Setup the directions array. We are not using up and down as we will handle
  // them later when we connect the layers.
  foreach(string dir, mapping info in DIRECTIONS)
    if(dir != "up" && dir != "down")
      directions += ({ ([ "dir" : dir, ]) + info });

  // Start from a random point within the inner maze area
  result = prandom(seed, MAX_X - MIN_X);
  seed = result[0];
  start_x = MIN_X + result[1];
  result = prandom(seed, MAX_Y - MIN_Y);
  seed = result[0];
  start_y = MIN_Y + result[1];

  if(!set_path(z, start_y, start_x))
    return;

  // Add the starting point to the stack, so we will begin generating from
  // here.
  stack += ({ (["x": start_x, "y": start_y]) });

  // While we still have points to process, keep generating the maze.
  while(sizeof(stack) > 0) {
    mapping current = stack[<1];
    mapping *shuffled_directions;
    int found_neighbour = 0;

    result = pshuffle(seed, directions);
    seed = result[0];
    shuffled_directions = result[1];

    // Try to find a neighbour to connect to. It does this because the map
    // is all walls at the start, so we need to set a path. We iterate over
    // the shuffled directions and try to find a room that is not on the
    // boundary and is a wall. If we find one, we change the current cell
    // to a path and the cell in the direction we are trying to set to a path.
    // Then we add the target cell to the stack to be processed later.
    foreach(mapping dir in shuffled_directions) {
      // Calculate the coordinates of the cell in the direction we are trying
      // to set. We add the direction's dx and dy to the current cell's x and y
      // to get the cell in the direction we are trying to set.
      int nx = current["x"] + (dir["dx"] * 2);
      int ny = current["y"] + (dir["dy"] * 2);

      if(oob(z, ny, nx))
        continue;

      // Check if the cell we're trying to set is not on the boundary and is a
      // wall.
      if(is_path(z, ny, nx))
        continue;

      room_count++;
      set_path(z, ny, nx);
      set_path(z, current["y"] + dir["dy"], current["x"] + dir["dx"]);
      stack += ({ (["x": nx, "y": ny]) });
      found_neighbour = 1;

      break;
    }

    // If we don't find a neighbour, we remove the current cell from the stack.
    if(!found_neighbour)
      stack = stack[0..<2];  // Remove the last element
  }
}

// Connect the layers together. This will connect the layers together by
// setting the down and up bits for the current layer and the layer below in
// the connections returned by find_layer_connections.
private void connect_layers() {
  int z;
  int i, sz;
  // Go layer by layer from the top to the second from the bottom, connecting
  // random spots in the current layer to random spots in the layer below.
  for(z = 0; z < dimensions[DEPTH]-1; z++) {
    mixed *connections;

    connections = find_layer_connections(z);
    for(i = 0, sz = sizeof(connections); i < sz; i++) {
      int *connection = connections[i];
      set_down(z, connection[0], connection[1]);
      set_up(z+1, connection[0], connection[1]);
    }
  }
}

// Finds all paths on the current layer that have a corresponding path set
// at the same x,y coordinates in the layer below. Returns an array of
// 1-3 random connections.
mixed *find_layer_connections(int z) {
  int *possible_down_exits;
  int *possible_up_exits;
  mixed *candidates;
  mixed *result;
  mixed *current = maze[z];
  mixed *below = maze[z+1];
  mixed *connections = ({});
  int num_connections;
  int x, y;

  candidates = ({});

  for(y = 0; y < dimensions[HEIGHT]; y++)
    for(x = 0; x < dimensions[WIDTH]; x++) {
      int *candidate = ({ z+1, y, x });

      if(is_path(z, y, x) && is_path(candidate[0], candidate[1], candidate[2]))
        candidates += ({ ({ candidate[1..]... }) });
    }

  if(sizeof(candidates) == 0)
    return null;

  result = prandom(seed, 3);
  seed = result[0];
  num_connections = result[1] + 1;

  while(num_connections--) {
    result = pelement_of(seed, candidates);
    seed = result[0];
    connections += ({ result[1] });
  }

  return connections;
}

// Check if the coordinates are out of bounds.
public int oob(int z, int y, int x) {
  return z < MIN_Z || z > MAX_Z ||
         y < MIN_Y || y > MAX_Y ||
         x < MIN_X || x > MAX_X;
}

// Clear a wall at the given coordinates.
private string clear_wall(int z, int y, int x) {
  string result;

  if(oob(z, y, x))
    return 0;

  result = maze[z][y][x] = clear_bit(maze[z][y][x], WALL);

  return result;
}

private string set_wall(int z, int y, int x) {
  string result;

  if(oob(z, y, x))
    return 0;

  clear_path(z, y, x);
  result = maze[z][y][x] = set_bit(maze[z][y][x], WALL);

  return result;
}

// Check if the coordinates are a wall.
public int is_wall(int z, int y, int x) {
  if(oob(z, y, x))
    return 0;

  return test_bit(maze[z][y][x], WALL);
}

// Clear a path at the given coordinates.
private string clear_path(int z, int y, int x) {
  string result;

  if(oob(z, y, x))
    return 0;

  result = maze[z][y][x] = clear_bit(maze[z][y][x], PATH);

  return result;
}

private string set_path(int z, int y, int x) {
  string result;

  if(oob(z, y, x))
    return 0;

  clear_wall(z, y, x);
  result = maze[z][y][x] = set_bit(maze[z][y][x], PATH);

  return result;
}

// Check if the coordinates are a path.
public int is_path(int z, int y, int x) {
  if(oob(z, y, x))
    return 0;

  return test_bit(maze[z][y][x], PATH);
}

// Clear a down connection at the given coordinates.
private string clear_down(int z, int y, int x) {
  string result;

  if(oob(z, y, x))
    return 0;

  result = maze[z][y][x] = clear_bit(maze[z][y][x], DOWN);

  return result;
}

// Set a down connection at the given coordinates.
private string set_down(int z, int y, int x) {
  string result;

  if(oob(z, y, x))
    return 0;

  result = maze[z][y][x] = set_bit(maze[z][y][x], DOWN);

  return result;
}

// Check if the coordinates are a down connection.
public int is_down(int z, int y, int x) {
  if(oob(z, y, x))
    return 0;

  return test_bit(maze[z][y][x], DOWN);
}

// Clear an up connection at the given coordinates.
private string clear_up(int z, int y, int x) {
  string result;

  if(oob(z, y, x))
    return 0;

  result = maze[z][y][x] = clear_bit(maze[z][y][x], UP);

  return result;
}

// Set an up connection at the given coordinates.
private string set_up(int z, int y, int x) {
  string result;

  if(oob(z, y, x))
    return 0;

  result = maze[z][y][x] = set_bit(maze[z][y][x], UP);

  return result;
}

// Check if the coordinates are an up connection.
public int is_up(int z, int y, int x) {
  if(oob(z, y, x))
    return 0;

  return test_bit(maze[z][y][x], UP);
}

// Initialize the maze. This will initialize the maze by allocating the maze
// array and setting all of the elements to a wall.
private void init_maze() {
  mixed *arr;
  int y, z;

  arr = allocate(dimensions[DEPTH]);
  for(z = 0; z < dimensions[DEPTH]; z++) {
    arr[z] = allocate(dimensions[HEIGHT]);
    for(y = 0; y < dimensions[HEIGHT]; y++)
      arr[z][y] = allocate(dimensions[WIDTH], set_bit("", WALL));
  }

  maze = arr;
}

// Setup the short descriptions for the maze.
private void setup_shorts() {
  short_descriptions = ({
    "Twisting Path",
    "Narrow Corridor",
    "Dark Passage",
    "Endless Hallway",
  });

  default_short_description = short_descriptions[0];
}

// Setup the long descriptions for the maze.
private void setup_longs() {
  long_descriptions = ({
    "The maze twists and turns, with walls of stone looming on either side.",
    "A narrow corridor stretches ahead, the air thick with mystery.",
    "You find yourself in a dark passage, the light barely reaching the walls.",
    "The hallway seems to go on forever, with no end in sight.",
  });

  default_long_description = "You stand in a maze, surrounded by high walls that "
    "block your view of the outside.";
}

// Setup the exits for the maze. This will setup the exits for the maze by
// adding the exits to the room.
public void setup_exits(object room) {
  int *coords = room->get_virtual_coordinates();
  int x, y, z, layer;
  int *adjacent;
  int max_x, max_y, max_z;
  string file = query_file_name(room);
  string room_type;
  string test;

  x = coords[2];
  y = coords[1];
  z = coords[0];

  max_x = get_map_width();
  max_y = get_map_height();
  max_z = get_map_depth();

  layer = abs(z);

  if(is_wall(layer, y, x))
    return;

  room_type = maze[layer][y][x];

  foreach(string dir, mapping info in DIRECTIONS) {
    adjacent = ({ z + DIRECTIONS[dir]["dz"], y + DIRECTIONS[dir]["dy"], x + DIRECTIONS[dir]["dx"] });

    if(!is_path(abs(adjacent[0]), adjacent[1], adjacent[2]))
      continue;

    if(dir == "up" && !is_up(abs(z), y, x))
      continue;
    else if(dir == "down" && !is_down(abs(z), y, x))
      continue;

    room->add_exit(dir, sprintf("%d,%d,%d", adjacent[2], adjacent[1], adjacent[0]));
  }

  // Set the exits to the maze exits
  // Set the exit to the forest
  // Do we have a valid exit to the forest?
  if(exits_out[FOREST]) {
    test = sprintf("%d,%d,%d", exits_out[FOREST][2], exits_out[FOREST][1], exits_out[FOREST][0]);
    if(file == test)
      room->add_exit("north", sprintf("../forest/%d,%d,%d",
        external_entrances[FOREST][2], external_entrances[FOREST][1], external_entrances[FOREST][0]));
  }

  // Set the exit to the wastes
  // Do we have a valid exit to the wastes?
  if(exits_out[WASTES]) {
    test = sprintf("%d,%d,%d", exits_out[WASTES][2], exits_out[WASTES][1], exits_out[WASTES][0]);
    if(file == test)
      room->add_exit("east", sprintf("../wastes/%d,%d,%d",
        external_entrances[WASTES][2], external_entrances[WASTES][1], external_entrances[WASTES][0]));
  }

  // Set the room icon for the centre of the room. Use the red X emoji.
  if(x == query_centre_room(layer)[2] && y == query_centre_room(layer)[1] && z == -query_centre_room(layer)[0])
    room->set_room_icon("🧭");
}

private nosave string wastes_daemon = "/d/wastes/wastes_daemon";
// Determine the exits and entrances for the maze. This will determine the
// exits and entrances for the maze by finding a valid entrance from the
// forest and a valid entrance from the wastes.
void determine_exits_and_entrances() {
  int *possible, state_steps, selected;
  mixed *result;
  int start, height, wastes_height;
  int minimum, size;
  int last;
  int step;
  int hole;
  mixed *path_neighbours;
  int it = 0;

  // Find a valid entrance from the forest
  // printf("Determining forest entrance and exit.\n");
  // These are the only columns that can have an entrance in the forest.
  possible = ({ 24, 25, 26, 27, });
  // This is the minimum width the maze has to be to have a potential exit into
  // the forest.
  minimum = possible[<1] - possible[0] + 1;

  // printf("Minimum width: %d\n", minimum);
  // printf("Maze width: %d\n", dimensions[WIDTH]);
  // Do we have enough width to have a potential exit into the forest?

  if(dimensions[WIDTH] >= minimum) {
    // This is the x coordinate of the last room in the forest that can have an
    // entrance.
    last = possible[<1];

    result = pelement_of(seed, possible);
    seed = result[0];
    selected = result[1];

    // This is the difference between the last room x coordinate in the forest
    // to the x coordinate of the selected room.
    state_steps = last - selected;
    // printf("Sizeof external_entrances: %d\n", sizeof(external_entrances));
    external_entrances[FOREST] = ({ 0, 11, selected });

    // Now let's carve out an exit path from the maze to the forest.
    hole = dimensions[WIDTH]-1-state_steps;
    exits_out[FOREST] = ({ 0, 0, hole});

    step = 0;
    while(step < dimensions[HEIGHT]-1 && is_wall(0, step, hole)) {
      set_path(0, step, hole);
      path_neighbours = find_neighbour_path(0, step, hole, FOREST);
      if(sizeof(path_neighbours)) {
        connect_to_neighbour_path(({ 0, step, hole }), path_neighbours);
        break ; // We've found a path to the forest
      }
      step++;
    }
  }

  // Find a valid entrance from the wastes

  // In the wastes, we need to start from the third row from the top, because
  // the first row leads west to the forest and we want enough distance from
  // the top of the maze to not be right in the corner.

  // We need to make sure that both the maze and the wastes have a suitable
  // height to connect to each other. Since the wastes is configrable for
  // different heights, we need to find the minimum height of the maze and the
  // wastes to connect to each other.

  // Maze height
  height = dimensions[HEIGHT];
  // Wastes height
  wastes_height = wastes_daemon->get_map_height();

  // The minimum height of the maze and the wastes is the height we can use.
  minimum = min(({ height, wastes_height }));

  // Subtract 3 from the minimum height to leave a gap of 2 rows between the
  // top of the wastes and the entrance to the maze.
  minimum -= 3;

  // Now we need to determine if we have enough height to have a valid
  // entrance and exit.
  if(height >= minimum && wastes_height >= minimum) {
    // Now we need to find a possible y coordinate in the wastes that is not
    // impassable.
    possible = ({});
    for(step = 1; step < minimum-1; step++) {
      if(wastes_daemon->determine_room_type(0, step) != "impassable")
        possible += ({ step });
    }

    if(sizeof(possible)) {
      // Select a random y coordinate from the possible coordinates
      result = pelement_of(seed, possible);
      seed = result[0];
      selected = result[1];
      hole = selected-1;

      // Record the entrance and exit coordinates
      external_entrances[WASTES] = ({ 0, selected, 0 });
      exits_out[WASTES] = ({ 0, hole, dimensions[WIDTH]-1 });

      // Now carve a path from the exit to the entrance
      step = dimensions[WIDTH]-1;
      while(is_wall(0, hole, step)) {
        it++;

        set_path(0, hole, step);
        path_neighbours = find_neighbour_path(0, hole, step, WASTES);
        if(sizeof(path_neighbours)) {
          connect_to_neighbour_path(({ 0, hole, step }), path_neighbours);
          break;
        }

        step--;
      }
    }
  }
}

// Connect to a neighbour path. This will connect to a neighbour path by
// carving out a path between the current coordinates and the destination
// coordinates. This function is used to connect the maze to the forest and
// the wastes.
private void connect_to_neighbour_path(int *curr, mixed *neighbours) {
  int *mods = neighbours[0];
  int *dest = neighbours[1];
  int new_x, new_y, new_z;

  new_z = curr[0];
  new_y = curr[1];
  new_x = curr[2];

  do {
    new_z += mods[0];
    new_y += mods[1];
    new_x += mods[2];

    set_path(new_z, new_y, new_x);
  } while(
    new_z != dest[0] ||
    new_y != dest[1] ||
    new_x != dest[2]);
}

// Find a neighbour path. This will find a neighbour path by checking the
// surrounding coordinates for a path. This function is used to find a path
// to connect the maze to the forest and the wastes.
mixed *find_neighbour_path(int z, int y, int x, int source) {
  int *neighbour;
  int *current = ({ z, y, x });
  int state_steps, coord;
  int max_steps = 5;
                      // North,           East,          South,         West
  int *directions = ({ ({ 0, -1, 0 }), ({ 0, 0, 1 }), ({ 0, 1, 0 }), ({ 0, 0, -1 }) });
  mixed *result;

  // North
  result = allocate(2);
  if(source != FOREST) {
    result[0] = directions[NORTH];
    state_steps = 0;
    while(state_steps++ < max_steps) {
      coord = y-(state_steps*1);
      if(coord < 0)
        break;

      neighbour = ({ z, coord, x });
      if(is_path(z, coord, x)) {
        result[1] = neighbour;
        return result;
      }
    }
  }

  // South
  result = allocate(2);
  result[0] = directions[SOUTH];
  state_steps = 0;
  while(state_steps++ < max_steps) {
    coord = y+(1*state_steps);
    if(coord >= dimensions[HEIGHT])
      break;

    neighbour = ({ z, coord, x });
    if(is_path(z, coord, x)) {
      result[1] = neighbour;
      return result;
    }
  }

  // West
  result = allocate(2);
  result[0] = directions[WEST];
  state_steps = 0;
  while(state_steps++ < max_steps) {
    coord = x-(1*state_steps);
    if(coord < 0)
      break;

    neighbour = ({ z, y, coord });
    if(is_path(z, y, coord)) {
      result[1] = neighbour;
      return result;
    }
  }

  // East
  result = allocate(2);
  if(source != WASTES) {
    result[0] = directions[EAST];
    state_steps = 0;
    while(state_steps++ < max_steps) {
      coord = x+(1*state_steps);
      if(coord >= dimensions[WIDTH])
        break;

      neighbour = ({ z, y, coord });
      if(is_path(z, y, coord)) {
        result[1] = neighbour;
        return result;
      }
    }
  }

  return null;
}

// Determine the centre room of a layer of the maze. It is the room that is
// closest to the centre of the maze that is a path room.
private void determine_centre_room(int z) {
  // Initialise the centre room to the middle of the maze
  int *centre = ({ z, MAX_Y/2, MAX_X/2, });
  int n, s, e, w;

  n = s = e = w = 0;

  // Now we need to find the closest path room to the centre of the maze
  // and set that as the centre room.
  while(centre[0]-w > MIN_X && centre[0]+e < MAX_X &&
        centre[1]-n > MIN_Y && centre[1]+s < MAX_Y) {

    //North
    if(is_path(centre[0], centre[1]-n, centre[2])) {
      centre[1] -= n;
      break ; // We've found the closest north path room
    } else n++;
    // South
    if(is_path(centre[0], centre[1]+s, centre[2])) {
      centre[1] += s;
      break ; // We've found the closest south path room
    } else s++;
    // West
    if(is_path(centre[0], centre[1], centre[2]-w)) {
      centre[2] -= w;
      break ; // We've found the closest west path room
    } else w++;
    // East
    if(is_path(centre[0], centre[1], centre[2]+e)) {
      centre[2] += e;
      break ; // We've found the closest east path room
    } else e++;
  }

  if(centre[2] - w <= MIN_X ||
     centre[2] + e >= MAX_X ||
     centre[1] - n <= MIN_Y ||
     centre[1] + s >= MAX_Y)
    error("Ran out of bounds determining centre room.");

  centre_rooms[z] = centre;
}

// Query the centre rooms of the maze.
mixed *query_centre_rooms() {
  return centre_rooms;
}

// Query the centre room of a layer of the maze.
int *query_centre_room(int z) {
  if(z < 0 || z >= dimensions[DEPTH])
    return 0;

  return centre_rooms[z];
}

// Query the north exit of the maze to the forest.
int *query_north_exit() {
  return exits_out[FOREST];
}

// Query the east exit of the maze to the wastes.
int *query_east_exit() {
  return exits_out[WASTES];
}

// Query the north entrance of the maze from the forest.
int *query_north_entrance() {
  return external_entrances[FOREST];
}

// Query the east entrance of the maze from the wastes.
int *query_east_entrance() {
  return external_entrances[WASTES];
}

// Setup the short description for a room.
public void setup_short(object room, string file) {
  int x, y, z;

  sscanf(file, "%d,%d,%d", x, y, z);
  room->set_short(element_of(short_descriptions) + " (Level "+(abs(z)+1)+")");
}

// Setup the long description for a room.
public void setup_long(object room, string file) {
  room->set_long(element_of(long_descriptions));
}

// Get the number of rooms in the maze.
int get_room_count() {
  return room_count;
}

// Display the maze.
void display_maze() {
  int x, y, z;
  string out = "";

  for(z = 0; z < dimensions[DEPTH]; z++) {
    out = "";
    for(y = 0; y < dimensions[HEIGHT]; y++) {
      for(x = 0; x < dimensions[WIDTH]; x++) {
        if(is_wall(z, y, x))
          out += " ";
        else
          out += "#";
      }
      out += "\n";
    }
    printf("\n%s", out);
  }
}

// Display a layer of the maze.
void display_layer(int z) {
  int x, y;
  string out = "";
  mixed *layer = maze[z];

  printf("\n");
  if(sizeof(layer) != dimensions[HEIGHT])
    error("Layer has the wrong height.");

  for(y = 0; y < dimensions[HEIGHT]; y++) {
    out = "";
    if(sizeof(layer[y]) != dimensions[WIDTH])
      error("Layer has the wrong width.");

    for(x = 0; x < dimensions[WIDTH]; x++) {
      if(is_wall(z, y, x))
        out += " ";
      else
        out += "#";
    }
    printf("%s\n", out);
  }
}

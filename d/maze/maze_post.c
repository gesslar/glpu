inherit STD_VIRTUAL_MAP ;

private nosave int *dimensions = ({25, 25}); // width, height
private nosave int WIDTH = 0, HEIGHT = 1 ;
private nosave mixed *maze ;

private nosave int MIN_X = 0, MIN_Y = 1, MAX_X = 2, MAX_Y = 3 ;
private nosave int WALL = 1 ;
private nosave int PATH = 0 ;

private void generate_maze() ;
private mixed *shuffle_array(mixed *arr) ;
private void clear_cell(int x, int y) ;
private varargs void set_wall(int x, int y) ;
private void trim_maze() ;
private void display_maze() ;

void generate_maze() {
  // Boundaries are 1 cell inside the maze for all sides
  int *boundaries = ({ 1, 1, dimensions[WIDTH]-2, dimensions[HEIGHT]-2 }) ;
  int x, y, start_x, start_y ;
  mixed *stack = ({}) ;
  mapping *directions = ({
    (["dx":  0, "dy": -1]),
    (["dx":  1, "dy":  0]),
    (["dx":  0, "dy":  1]),
    (["dx": -1, "dy":  0])
  }) ;
  int result ;

  maze = allocate(dimensions[HEIGHT]) ;
  for(y = 0; y < dimensions[HEIGHT]; y++)
    maze[y] = allocate(dimensions[WIDTH], WALL) ;

  // Start from a random point within the inner maze area
  result = random(boundaries[MAX_X] - boundaries[MIN_X]) ;
  start_x = boundaries[MIN_X] + result ;
  result = random(boundaries[MAX_Y] - boundaries[MIN_Y]) ;
  start_y = boundaries[MIN_Y] + result ;

  clear_cell(start_x, start_y) ;
  stack += ({ (["x": start_x, "y": start_y]) }) ;

  while(sizeof(stack) > 0) {
    mapping current = stack[<1] ;
    mapping *shuffled_directions = shuffle_array(directions) ;
    int found_neighbor = 0 ;

    foreach(mapping dir in shuffled_directions) {
      int nx = current["x"] + dir["dx"] * 2 ;
      int ny = current["y"] + dir["dy"] * 2 ;

      // Check if the new position is within the inner boundaries
      if(nx >= boundaries[MIN_X] && nx <= boundaries[MAX_X] &&
         ny >= boundaries[MIN_Y] && ny <= boundaries[MAX_Y]) {

        // Check if the cell two steps away is a wall and not on the boundary
        if(maze[ny][nx] == WALL &&
           nx > 0 && nx < dimensions[WIDTH]-1 &&
           ny > 0 && ny < dimensions[HEIGHT]-1) {

          clear_cell(current["x"] + dir["dx"], current["y"] + dir["dy"]) ;
          clear_cell(nx, ny) ;
          stack += ({ (["x": nx, "y": ny]) }) ;
          found_neighbor = 1 ;
          break ;
        }
      }
    }

    if(!found_neighbor)
      stack = stack[0..<2];  // Pop the last element
  }

  // This aslgorithm sometimes creates a second wall layer just on the inside
  // of any number of sides. We need to remove them. Know that this can
  // result in a smaller maze than requested.
  trim_maze() ;
}

private mixed *shuffle_array(mixed *arr) {
  mixed *shuffled = allocate(sizeof(arr)) ;
  int i, j, temp ;

  for(i = 0; i < sizeof(arr); i++) {
    shuffled[i] = arr[i] ;
  }

  for(i = sizeof(shuffled) - 1; i > 0; i--) {
    int result = random(i + 1) ;

    j = result ;
    temp = shuffled[i] ;
    shuffled[i] = shuffled[j] ;
    shuffled[j] = temp ;
  }

  return shuffled ;
}

private varargs void clear_cell(int x, int y) {
  maze[y][x] = PATH ;
}

private varargs void set_wall(int x, int y) {
  maze[y][x] = WALL ;
}

// Find if the boundaries are doubled on any side and remove that row/column
// if necessary. We need to check inside the corners by 1 to see if they are
// walls, and if so, we need to remove that row/column.
void trim_maze() {
  int x, y ;
  int count ;

  // North wall
  count = 0 ;
  for(x = 0; x < dimensions[WIDTH]; x++) {
    if(maze[1][x] == WALL)
      count++ ;
  }
  if(count == dimensions[WIDTH]) {
    maze = maze[1..] ;
    dimensions[HEIGHT]-- ;
  }

  // South wall
  count = 0 ;
  for(x = 0; x < dimensions[WIDTH]; x++) {
    if(maze[dimensions[HEIGHT]-2][x] == WALL)
      count++ ;
  }
  if(count == dimensions[WIDTH]) {
    maze = maze[0..<2] ;
    dimensions[HEIGHT]-- ;
  }

  // West wall
  count = 0 ;
  for(y = 0; y < dimensions[HEIGHT]; y++) {
    if(maze[y][1] == WALL)
      count++ ;
  }
  if(count == dimensions[HEIGHT]) {
    int *tmp = allocate(dimensions[HEIGHT]) ;
    for(y = 0; y < dimensions[HEIGHT]; y++) {
      tmp[y] = maze[y][1..] ;
    }
    maze = tmp ;
    dimensions[WIDTH]-- ;
  }

  // East wall
  count = 0 ;
  for(y = 0; y < dimensions[HEIGHT]; y++) {
    if(maze[y][dimensions[WIDTH]-2] == WALL)
      count++ ;
  }
  if(count == dimensions[HEIGHT]) {
    int *tmp = allocate(dimensions[HEIGHT]) ;
    for(y = 0; y < dimensions[HEIGHT]; y++) {
      tmp[y] = maze[y][0..<2] ;
    }
    maze = tmp ;
    dimensions[WIDTH]-- ;
  }
}

void display_maze() {
  int x, y ;
  string out = "" ;

  for(y = 0; y < dimensions[HEIGHT]; y++) {
    for(x = 0; x < dimensions[WIDTH]; x++) {
      if(maze[y][x] == WALL) {
        out += "#" ;
      } else {
        out += " " ;
      }
    }
    out += "\n" ;
  }

  printf("%s", out) ;
}

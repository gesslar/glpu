inherit STD_DAEMON ;

void generate(int dim, int numDoors) ;
void add_outer_walls() ;
int add_entrance() ;
void add_inner_walls(int h, int minX, int maxX, int minY, int maxY, int gate) ;
void add_h_wall(int minX, int maxX, int y) ;
void add_v_wall(int minY, int maxY, int x) ;
string display() ;
int do_generate(string arg) ;

private mixed *grid ;
private int dimensions ;

void generate(int dim, int numDoors) {
  int ent ;
  string result = "" ;

  dimensions = dim ;
  grid = allocate(dimensions) ;
  for(int i = 0; i < dimensions; i++) {
    grid[i] = allocate(dimensions, "") ;
  }
  add_outer_walls() ;
  add_inner_walls(1, 1, dimensions - 2, 1, dimensions - 2, ent) ;
  ent = add_entrance() ;

  result = display() ;

  printf("%s\n", result) ;
}

void add_outer_walls() {
  for(int i = 0; i < dimensions; i++) {
    if(i == 0 || i == (dimensions - 1)) {
      for(int j = 0; j < dimensions; j++) {
        grid[i][j] = "w" ;
      }
    } else {
      grid[i][0] = "w" ;
      grid[i][dimensions - 1] = "w" ;
    }
  }
}

int add_entrance() {
  int x = random(dimensions - 2) + 1 ;
  int wallcheck = 1 ;

  grid[dimensions - 1][x] = "g" ;

  for(int y = dimensions - 2; y >= 0; y--) {
    if(grid[y][x] == "w")
      grid[y][x] = " " ;
    else
      break ;
  }
  return x ;
}

void add_inner_walls(int h, int minX, int maxX, int minY, int maxY, int gate) {
  if(h) {
    int y ;

    if(maxX - minX < 2)
      return ;

    y = (random(maxY - minY + 1) + minY) / 2 * 2 ;
    add_h_wall(minX, maxX, y) ;
    add_inner_walls(!h, minX, maxX, minY, y - 1, gate) ;
    add_inner_walls(!h, minX, maxX, y + 1, maxY, gate) ;
  } else {
    int x ;

    if(maxY - minY < 2)
      return ;

    x = (random(maxX - minX + 1) + minX) / 2 * 2 ;
    add_v_wall(minY, maxY, x) ;
    add_inner_walls(!h, minX, x - 1, minY, maxY, gate) ;
    add_inner_walls(!h, x + 1, maxX, minY, maxY, gate) ;
  }
}

void add_h_wall(int minX, int maxX, int y) {
  int hole = (random(maxX - minX + 1) + minX) / 2 * 2 + 1 ;
  for(int i = minX; i <= maxX; i++) {
    grid[y][i] = (i == hole) ? "" : "w" ;
  }
}

void add_v_wall(int minY, int maxY, int x) {
  int hole = (random(maxY - minY + 1) + minY) / 2 * 2 + 1 ;
  for(int i = minY; i <= maxY; i++) {
    grid[i][x] = (i == hole) ? "" : "w" ;
  }
}

string display() {
  string output = "" ;
  for(int i = 0; i < dimensions; i++) {
    for(int j = 0; j < dimensions; j++) {
      output += (grid[i][j] == "w") ? "#" :
      (grid[i][j] == "g") ? "G" : " " ;
    }
    output += "\n" ;
  }
  return output ;
}

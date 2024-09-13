/**
 * @file /d/wastes/wastes_daemon.c
 * @description Virtual map daemon for the wastes
 *
 * @created 2024-08-30 - Gesslar
 * @last_modified 2024-08-30 - Gesslar
 *
 * @history
 * 2024-08-30 - Gesslar - Created
 */

inherit STD_VIRTUAL_MAP;

inherit M_NOISE;

private void setup_wastes_shorts() ;
private void setup_wastes_longs() ;
private void generate_map() ;
private mixed *generate_terrain() ;
private mixed *generate_river(mixed *map) ;
varargs string determine_room_type(int z, int y, int x) ;
private void setup_dimensions() ;

private nosave string *wastes_shorts ;
private nosave string *wastes_longs ;
private nosave string wastes_long ;
private nosave int rot = 0 ;
private nosave int *dimensions ;
// private nosave mixed *dimension_config = ({ ({ 25, 75 }), ({ 15, 75 }) }) ;
private nosave mixed *dimension_config = ({ ({ 30, 1 }), ({ 30, 1 }) }) ;
private nosave int WIDTH = 0, HEIGHT = 1 ;
// private nosave int seed = BOOT_D->query_boot_number() ;
private nosave int seed = 42 ;

void setup() {
  // init_noise(dimensions[0], dimensions[1], BOOT_D->query_boot_number()) ;
  setup_dimensions() ;
  init_noise(dimensions[0], dimensions[1], seed) ;
  apply_map_generator((: generate_map :)) ;
  setup_wastes_shorts();
  setup_wastes_longs();
}

private void setup_dimensions() {
  mixed *result ;

  dimensions = allocate(2) ;

  result = prandom(seed, dimension_config[WIDTH][1]) ;
  seed = result[0] ;
  dimensions[WIDTH] = result[1] + dimension_config[WIDTH][0] ;

  // Height - 1 - 5
  result = prandom(seed, dimension_config[HEIGHT][1]) ;
  seed = result[0] ;
  dimensions[HEIGHT] = result[1] + dimension_config[HEIGHT][0] ;
}

protected mixed *generate_map() {
  mixed *map = generate_terrain();
  map = generate_river(map);

  return map;
}

private mixed *generate_terrain() {
  mixed map;
  int center_x = dimensions[WIDTH] / 2;
  int center_y = dimensions[HEIGHT] / 2;

 map = allocate(1) ;
  for(int z = 0; z < 1; z++) {
    map[z] = allocate(dimensions[HEIGHT]);
    for(int y = 0; y < dimensions[HEIGHT]; y++) {
      map[z][y] = allocate(dimensions[WIDTH]);
      for(int x = 0; x < dimensions[WIDTH]; x++) {
        // float noise_value =
        //     simplex2(to_float(x) /  5, to_float(y) /  5) +
        //     simplex2(to_float(x) / 10, to_float(y) / 10) +
        //     simplex2(to_float(x) / 15, to_float(y) / 15) ;
        mixed *result ;
        float noise_value ;

        float distance = sqrt(pow(x - center_x, 2) + pow(y - center_y, 2));
        // Adjust the 0.05 value to control strength
        float dampening_factor = 1.0 / (1.0 + distance * 0.05);

        noise_value = simplex2(
          to_float(x) / 25.0,
          to_float(y) / 15.0
        ) ;

        noise_value *= dampening_factor ;
        noise_value += 1.0 ;
        update_noise_range(noise_value); // Update min/max values
        map[z][y][x] = noise_value; // Store the noise value in the map
      }
    }
  }

  return map;
}

private mixed *generate_river(mixed map) {
  int river_width = dimensions[WIDTH] / 6 ;
  int river_height = dimensions[HEIGHT] ;
  // int river_start_x = (dimensions[WIDTH] / 2) - (river_width / 2) ;
  int river_start_x = dimensions[WIDTH] - (river_width * 3) ;
  int river_end_x = river_start_x + river_width ;
  int river_start_y = 0 ;
  int center_y = dimensions[HEIGHT] / 2 ;
  mapping counts = ([
    "shallow water": 0,
    "water": 0,
    "deep water": 0,
  ]) ;

  printf("River start: %d,%d\n", river_start_x, river_start_y);
  printf("River width: %d, height: %d\n", river_width, river_height);
  printf("Map size: %d,%d\n", dimensions[WIDTH], dimensions[HEIGHT]);

  for(int y = 0; y < river_height; y++) {
    for(int x = 0; x < river_width; x++) {
      float river_noise_value = simplex2(to_float(x) / 10.0, to_float(y) / 20.0);
      float bias = (to_float(y) - center_y) * 0.05;  // Bias toward a more vertical flow
      float river_threshold = 1.0 + river_noise_value + bias;

      // Introduce variability in the river's width
      if(river_threshold < 1.0) {
        if(river_threshold < 0.5) {
          // Shallow river
          map[0][river_start_y + y][river_start_x + x] = -0.25;
          counts["shallow water"]++;
        } else if(river_threshold < 0.75) {
          // Regular river
          map[0][river_start_y + y][river_start_x + x] = -0.5;
          counts["water"]++;
        } else if(river_threshold < 1.0) {
          // Deep river
          map[0][river_start_y + y][river_start_x + x] = -1.0;
          counts["deep water"]++;
        }
      }
    }
  }

  // Now a pass to ensure that there are no deep water at the edges.
  // Make them regular water.
  for(int y = 0; y < river_height; y++) {
    int west_found = false, east_found = false ;

    for(int x = 0; x < river_width; x++) {
      // Check the west side
      if(!west_found && map[0][river_start_y + y][river_start_x + x] == -1.0) {
        west_found = true;
        map[0][river_start_y + y][river_start_x + x] = -0.5;
        counts["deep water"]--;
        counts["water"]++;
      }

      // Check the east side
      if(!east_found && map[0][river_start_y + y][river_end_x - x] == -1.0) {
        east_found = true;
        map[0][river_start_y + y][river_end_x - x] = -0.5;
        counts["deep water"]--;
        counts["water"]++;
      }

      if(west_found && east_found)
        break;
    }

    west_found = east_found = false ;
  }

  return map;
}

private void setup_wastes_shorts() {
  wastes_shorts = ({
    "Barren Expanse",
    "Scorched Earth",
    "Desolate Stretch",
    "Rugged Terrain",
  });
}

private void setup_wastes_longs() {
  wastes_longs = ({
    "The wasteland stretches before you, a desolate expanse of cracked earth "
    "and scattered debris. The air is dry and carries the faint scent of decay, "
    "with the sun beating down mercilessly.",

    "This area of the wasteland is littered with remnants of a forgotten past, "
    "rusted metal and broken glass glinting in the harsh light. The ground is "
    "uneven, with deep fissures and patches of thorny vegetation.",

    "A chilling wind sweeps across the wasteland, stirring up dust and debris. "
    "The landscape is dotted with skeletal remains of long-dead trees, their "
    "twisted branches reaching out like bony fingers.",

    "Silence reigns in this part of the wasteland, broken only by the occasional "
    "whistle of the wind. The ground is barren, with only the hardiest of plants "
    "struggling to survive in the harsh conditions."
  });

  wastes_long =
    "The wasteland opens up into a vast, open area, where the horizon seems to "
    "stretch endlessly. The ground is a patchwork of cracked earth and sparse "
    "vegetation, with the sun casting long shadows. In the distance, you can "
    "see the remnants of a ruined structure, a ghost of what once was, standing "
    "as a testament to the desolation that surrounds you.";
}

public void setup_short(object room, string file) {
  room->set_short(element_of(wastes_shorts));
}

public void setup_long(object room, string file) {
  room->set_long(element_of(wastes_longs));
}

public void setup_exits(object room) {
  int *coords = room->get_virtual_coordinates();
  int x, y, z;
  int max_x, max_y;
  string current_room_type;
  string room_type;
  mapping exits;

  x = coords[2];
  y = coords[1];
  z = coords[0];

  max_x = get_map_width();
  max_y = get_map_height();

  current_room_type = determine_room_type(z, y, x);

  if(current_room_type == "impassable")
    return;

  // West
  if(x-1 >= 0) {
    room_type = determine_room_type(z, y, x-1);
    if(room_type != "impassable")
      room->add_exit("west", sprintf("%d,%d,%d", x-1, y, z));
  }

  // East
  if(x+1 < max_x) {
    room_type = determine_room_type(z, y, x+1);
    if(room_type != "impassable")
      room->add_exit("east", sprintf("%d,%d,%d", x+1, y, z));
  }

  // South
  if(y+1 < max_y) {
    room_type = determine_room_type(z, y+1, x);
    if(room_type != "impassable")
      room->add_exit("south", sprintf("%d,%d,%d", x, y+1, z));
  }

  // North
  if(y-1 >= 0) {
    room_type = determine_room_type(z, y-1, x);
    if(room_type != "impassable")
      room->add_exit("north", sprintf("%d,%d,%d", x, y-1, z));
  }

#if 0
  // Northeast
  if(x+1 < max_x && y-1 >= 0) {
    room_type = determine_room_type(z, y-1, x+1);
    if(room_type != "impassable")
      room->add_exit("northeast", sprintf("%d,%d,%d", x+1, y-1, z));
  }

  // Northwest
  if(x-1 >= 0 && y-1 >= 0) {
    room_type = determine_room_type(z, y-1, x-1);
    if(room_type != "impassable")
      room->add_exit("northwest", sprintf("%d,%d,%d", x-1, y-1, z));
  }

  // Southeast
  if(x+1 < max_x && y+1 < max_y) {
    room_type = determine_room_type(z, y+1, x+1);
    if(room_type != "impassable")
      room->add_exit("southeast", sprintf("%d,%d,%d", x+1, y+1, z));
  }

  // Southwest
  if(x-1 >= 0 && y+1 < max_y) {
    room_type = determine_room_type(z, y+1, x-1);
    if(room_type != "impassable")
      room->add_exit("southwest", sprintf("%d,%d,%d", x-1, y+1, z));
  }
#endif
  // Forest entrance
  if(x == 0 && y == 0 && z == 0)
    room->add_exit("west", "../forest/27,11,0");

  {
    string maze_entrance_file ;
    string maze_exit_file ;
    int *maze_entrance, *maze_exit ;

    maze_entrance = "/d/maze/maze_daemon"->query_east_entrance() ;
    if(maze_entrance[0] == z && maze_entrance[1] == y && maze_entrance[2] == x) {
      maze_entrance_file = sprintf("%d,%d,%d",
        maze_entrance[2], maze_entrance[1], maze_entrance[0]) ;

      maze_exit = "/d/maze/maze_daemon"->query_east_exit() ;
      maze_exit_file = sprintf("%d,%d,%d", maze_exit[2], maze_exit[1], maze_exit[0]) ;

      room->add_exit("west", "/d/maze/"+maze_exit_file);
    }
  }
}

private nosave string *terrain_types = ({ "rocky", "sandy", "impassable","grassy" }) ;
private nosave mixed *thresholds = ({
  ({ 0.3, "impassable" }),   //  0 -  30% (larger region of impassable)
  ({ 1.2, "sandy" }),        // 30 -  60% (wider sandy area)
  ({ 1.7, "rocky" }),        // 60 -  85% (more gradual transition to rocky)
  ({ 2.0, "impassable" }),   // 85 - 100% (end with impassable)
});

varargs string determine_room_type(int z, int y, int x) {
  int value ;
  string room_type ;

  value = get_room_type(z, y, x);
  if(nullp(value))
    return null ;

  if(value == -1.0)
    return "deep water";
  else if(value == -0.5)
    return "water";
  else if(value == -0.25)
    return "shallow water";
  else
    foreach(mixed threshold in thresholds) {
      int threshold_value = threshold[0];
      string terrain_type = threshold[1];

      if(value <= threshold_value) {
        room_type = terrain_type;
        break;
      }
    }

  return room_type ;
}

public void setup_room_type(object room) {
  int *coords = room->get_virtual_coordinates();
  string room_type;
  room_type = determine_room_type(coords...) ;

  if(nullp(room_type))
    return;

  room->set_room_environment(room_type);
}

void print_row(int y) {
  int x;
  mixed *map = query_map_data();

  for(x = 0; x < sizeof(map); x++) {
    printf("%d ", map[y][x]);
  }
  printf("\n");
}

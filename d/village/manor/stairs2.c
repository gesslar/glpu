/**
 * @file /d/village/house/stairs2.c
 * @description The second set of stairs in the manor.
 *
 * @created 2024-09-13 - Gesslar
 * @last_modified 2024-09-13 - Gesslar
 *
 * @history
 * 2024-09-13 - Gesslar - Created
 */

inherit __DIR__ "manor" ;

void setup() {
  set_short("The Second Set of Stairs");
  set_long("The second set of stairs continues the theme of excess, with a "
  "lavish carpet that seems to swallow your feet. The walls are painted a "
  "shocking pink, adorned with gaudy floral patterns that clash with the "
  "ornate banister. At the top, a large, extravagant chandelier hangs, its "
  "crystals glinting in the light as you ascend to the next level of this "
  "ostentatious abode.") ;

  set_exits(([
    "down" : "landing",
    "up"   : "hall4",
  ])) ;

  set_items(([
    "carpet" :
    "The lavish carpet is thick and plush, providing a soft surface "
    "underfoot.",
    "walls" :
    "The walls are painted a shocking pink, adorned with gaudy floral "
    "patterns that clash with the decor.",
    "banister" :
    "The ornate banister is intricately carved, adding a touch of elegance to "
    "the stairs.",
    "chandelier" :
    "The large chandelier at the top sparkles with crystals, casting "
    "shimmering light down the stairs.",
  ])) ;
}

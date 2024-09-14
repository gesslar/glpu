/**
 * @file /d/village/house/stairs1.c
 * @description The first set of stairs in the manor.
 *
 * @created 2024-09-13 - Gesslar
 * @last_modified 2024-09-13 - Gesslar
 *
 * @history
 * 2024-09-13 - Gesslar - Created
 */

inherit __DIR__ "manor" ;

void setup() {
  set_short("The First Set of Stairs");
  set_long("These stairs are a flamboyant ascent, lined with plush, velvet "
  "runners in a garish shade of red. The walls are adorned with oversized, "
  "gold-framed mirrors that reflect your image as you climb. Ornate sconces "
  "flicker with candlelight, casting dancing shadows that add to the dramatic "
  "effect. Each step creaks underfoot, echoing the opulence of the manor as "
  "you make your way up to the landing.") ;

  set_exits(([
    "down": "hall3",
    "up"  : "landing",
  ])) ;

  set_items(([
    "runners" :
    "The plush, velvet runners in a garish shade of red add a touch of luxury "
    "to the stairs.",
    "mirrors" :
    "The oversized, gold-framed mirrors reflect your image, creating a sense "
    "of grandeur.",
    "sconces" :
    "The ornate sconces flicker with candlelight, casting dancing shadows "
    "along the walls.",
  ])) ;
}

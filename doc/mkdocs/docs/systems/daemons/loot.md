# Loot Daemon (LOOT_D)

The Loot Daemon (LOOT_D) is a daemon that is responsible for managing the loot
that is dropped by mobs when they are killed, or treasure chests, or other
sources of loot.

## Loot module

To use the loot system, you need to inherit `M_LOOT` in your mob or object.
This will give you access to the API to add to or set the loot table.

### API

#### `void add_loot(mixed item, float chance)`

Adds a new item or set of items to the loot table.

##### Parameters

* item (mixed): The item or items to be added. Can be one of the following:
    * string: A single item represented by its file path.
    * mapping: A weighted mapping of items.
    * function: A function that returns an item or set of items.
    * array: An array of items, which can include strings, mappings, or nested
      arrays.

* chance (float): The chance of this loot entry being selected, as a
  percentage. If omitted, defaults to 100%. Integer values are converted to
  floats. Values are clamped to the range 0.0 to 100.0.

##### Behavior

* If the item is null, the function returns without adding anything.
* If the chance is null, it defaults to 100%.
* If the chance is not a float (after conversion), the function returns without
  adding anything.
* For string, mapping, and function types, the item is added directly to the
  loot table.
* For arrays, each element of the array is added as a separate entry in the
  loot table, all with the same chance.

##### Example usage

    add_loot("/obj/sword", 75.0);
    add_loot(([ "/obj/gem" : 1, "/obj/coin" : 3 ]), 50.0);
    add_loot(({ "/obj/potion", ({ "healing", 50 }) }), 25.0);
    add_loot(({ "/obj/sword", "/obj/axe", "/obj/mace" }), 80.0);

#### `void set_loot_table(mixed *table)`

Sets the entire loot table for the object.

##### Parameters

* table (mixed *): An array representing the complete loot table. Each element
should be an array containing two elements:
    1. The item or items (can be a string, mapping, function, or array)
    2. The chance as a float

##### Behavior

* If the provided table is not an array, the function returns without changing
  the loot table.
* This function completely replaces the existing loot table with the new one.

##### Example usage

    set_loot_table(({
        ({ "/obj/sword", 75.0 }),
        ({ ({ "/obj/potion", ({ "healing", 50 }) }), 25.0 }),
        ({ ([ "/obj/gem" : 1, "/obj/coin" : 3 ]), 50.0 })
    }));

#### `mixed *query_loot_table()`

Retrieves the current loot table.

##### Returns

A copy of the current loot table array.

***Note***: This function returns a copy to prevent direct modification of the
loot table.

##### Example usage

    mixed *current_table = query_loot_table();

***Note***: The loot table is stored as a private nosave variable, meaning it
is not saved between reboots and is not accessible outside the object.

## Loot Daemon

`LOOT_D` processes the array of entries from the object, where each entry is an
array containing two elements:

1. The item or items to be dropped
2. The chance (as a float) of this entry being selected

The item element can be configured in several ways:

* Simple String - A single item represented by its file path.

*Example*

    "/obj/sword"

* Array of Strings - Multiple items, one of which will be randomly selected.

*Example*

    ({ "/obj/sword", "/obj/axe", "/obj/mace" })

* Array with Item and Arguments - An item with additional creation arguments.

*Example*

    ({ "/obj/potion", ({ "healing", 50 }) })

Weighted Mapping - A mapping where keys are items and values are their weights.

*Example*

    ([ "/obj/gem" : 1, "/obj/coin" : 3 ])

* Complex Weighted Mapping - A mapping where keys can be any of the above item
  types.

*Example*

    ([
        "/obj/gem" : 1,
        ({ "/obj/sword", "/obj/axe" }) : 2,
        ({ "/obj/potion", ({ "strength", 3 }) }) : 3
    ])

* Function - A function that returns any of the above types.

*Example*

    (: generate_random_item :)

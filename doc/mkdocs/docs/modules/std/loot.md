To use the loot system, you need to inherit `M_LOOT` in your mob or object.
This will give you access to the API to add to or set the loot table. Refer to
the documentation on [`LOOT_D`](../../daemons/loot.md) for more details on how the
system is executed.

## `add_loot`

Add a new item or set of items to the loot table.

* If the item is null, the function returns without adding anything.
* If the chance is null, it defaults to 100%.
* If the chance is not a float (after conversion), the function returns without
  adding anything.
* For string, mapping, and function types, the item is added directly to the
  loot table.
* For arrays, each element of the array is added as a separate entry in the
  loot table, all with the same chance.

`#!C void add_loot(mixed item, float chance)`

* `item (mixed)` - The item or items to be added. Can be one of the following:
  * `string` - A single item represented by its file path.
  * `mapping` - A weighted mapping of items.
  * `function` - A function that returns an item or set of items.
  * `array` - An array of items, which can include strings, mappings, or nested
    arrays.
* `chance (float)` - The chance of this loot entry being selected, as a
  percentage. If omitted, defaults to 100%. Integer values are converted to
  floats. Values are clamped to the range 0.0 to 100.0.

!!! example

    ``` c
    add_loot("/obj/sword", 75.0);
    add_loot(([ "/obj/gem" : 1, "/obj/coin" : 3 ]), 50.0);
    add_loot(({ "/obj/potion", ({ "healing", 50 }) }), 25.0);
    add_loot(({ "/obj/sword", "/obj/axe", "/obj/mace" }), 80.0);
    ```

## `set_loot_table`

Set the entire loot table for the object.

* If the provided table is not an array, the function returns without changing
  the loot table.
* This function completely replaces the existing loot table with the new one.

`#!C void set_loot_table(mixed *table)`

* `table (mixed *)` - An array representing the complete loot table. Each element
  should be an array containing two elements:
    * The item or items (can be a string, mapping, function, or array)
    * The chance as a float

!!! example

    ``` c
    set_loot_table(({
        ({ "/obj/sword", 75.0 }),
        ({ "/obj/potion", ({ "healing", 50 }) }),
        ({ ([ "/obj/gem" : 1, "/obj/coin" : 3 ]), 50.0 })
    }));
    ```

## `query_loot_table`

Retrieve the current loot table.

!!! note ""

    This function returns a copy to prevent direct modification of the loot
    table.

`#!C mixed *query_loot_table()`


!!! example

    ``` c
    mixed *current_table = query_loot_table();
    ```

`LOOT_D` is responsible for managing the loot that is dropped by mobs when
they are killed, or treasure chests, or other sources of loot. Refer to the
documentation on [`M_LOOT`](../modules/std/loot.md) for more details on how to
use the loot system.

`LOOT_D` processes the array of entries from the object, where each entry is an
array containing two elements:

* The item or items to be dropped
* The chance (as a float) of this entry being selected

The item element can be configured in several ways:

* **Simple String** - A single item represented by its file path.

      `#!c "/obj/sword"`

* **Array of Strings** - Multiple items, one of which will be randomly
  selected.

      `#!c ({ "/obj/sword", "/obj/axe", "/obj/mace" })`

* **Array with Item and Arguments** - An item with additional setup arguments.

      `#!c ({ "/obj/potion", ({ "healing", 50 }) })`

* **Weighted Mapping** - A mapping where keys are items and values are their
  weights.

      `#!c ([ "/obj/gem" : 1, "/obj/coin" : 3 ])`

* **Complex Weighted Mapping** - A mapping where keys can be any of the above
  item types.

    ``` c
    ([
        "/obj/gem" : 1,
        ({ "/obj/sword", "/obj/axe" }) : 2,
        ({ "/obj/potion", ({ "strength", 3 }) }) : 3
    ])
    ```

* **Function** - A function that returns any of the above types.

      `#!c (: generate_random_item :)`

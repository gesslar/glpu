/**
 * @file /obj/loot/loot.c
 * @description Base loot inheritable. This is for vendor trash, etc.
 *
 * @created 2024-08-20 - Gesslar
 * @last_modified 2024-08-21 - Gesslar
 *
 * @history
 * 2024-08-20 - Gesslar - Created
 * 2024-08-21 - Gesslar - Updated to include virtual_setup function
 * 2024-08-21 - Gesslar - Updated to use mass instead of weight and handle value as mixed array
 */

inherit STD_ITEM;

void mudlib_setup() {
    add_id("loot");
}

void virtual_setup(mixed args...) {
    mapping data;

    if (!args || !mapp(args[0]))
        return;

    data = args[0];

    // Set basic properties
    if (!nullp(data["id"]))
        set_id(data["id"]);
    if (!nullp(data["adj"]))
        set_adj(data["adj"]);
    if (!nullp(data["name"]))
        set_name(data["name"]);
    if (!nullp(data["short"]))
        set_short(data["short"]);
    if (!nullp(data["long"]))
        set_long(data["long"]);
    if (!nullp(data["mass"]))
        set_mass(data["mass"]);

    // Set value
    if (!nullp(data["value"]) && pointerp(data["value"]) && sizeof(data["value"]) == 2)
        set_value(data["value"][0], data["value"][1]);

    // Set material
    // if (!nullp(data["material"]))
    //     set_material(data["material"]);

    // Set additional properties
    // if (!nullp(data["properties"]) && mapp(data["properties"])) {
    //     foreach (string key, mixed value in data["properties"]) {
    //         set_property(key, value);
    //     }
    // }

    // Add additional IDs
    if (!nullp(data["additional_ids"]) && pointerp(data["additional_ids"])) {
        foreach (string id in data["additional_ids"]) {
            add_id(id);
        }
    }

    // Set any custom functions or properties
    if (!nullp(data["custom_setup"]) && functionp(data["custom_setup"])) {
        (*data["custom_setup"])(this_object());
    }
}

// Additional loot-specific functions can be added here

/**
 * @file /std/living/attributes.c
 * @description Player attributes.
 *
 * @created 2024/07/30 - Gesslar
 * @last_modified 2024/07/30 - Gesslar
 *
 * @history
 * 2024/07/30 - Gesslar - Created
 */

private nomask nosave mapping default_attributes = ([]) ;
private nomask mapping attributes = ([]) ;

void init_attributes() {
    string key ;
    mixed data ;

    default_attributes = json_decode(read_file(mud_config("ATTRIBUTES"))) ;

    foreach(key, data in default_attributes) {
        if(!of(key, attributes)) {
            attributes[key] = (data["max"] - data["min"]) / 2 + data["min"] ;
        }
    }

    foreach(key, data in attributes) {
        if(!of(key, default_attributes)) {
            map_delete(attributes, key) ;
        }
    }
}

int set_attribute(string key, int value) {
    if(!of(key, attributes)) {
        return null ;
    }

    attributes[key] = value;

    return attributes[key];
}

int query_attribute(string key) {
    if(!of(key, attributes)) {
        return null ;
    }

    return attributes[key];
}

int modify_attribute(string key, int value) {
    if(!of(key, attributes)) {
        return null ;
    }

    attributes[key] += value;

    return attributes[key];
}

mapping query_attributes() {
    return copy(attributes) ;
}

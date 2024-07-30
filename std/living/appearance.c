/**
 * @file /std/living/appearance.c
 * @description Handles the appearance of living objects
 *
 * @created 2024/07/30 - Gesslar
 * @last_modified 2024/07/30 - Gesslar
 *
 * @history
 * 2024/07/30 - Gesslar - Created
 */

string hair, hair_colour, eyes, eye_colour ;

string query_hair() {
    return hair;
}

string query_hair_colour() {
    return hair_colour;
}

string query_eyes() {
    return eyes;
}

string query_eye_colour() {
    return eye_colour;
}

int set_hair(string str) {
    hair = str;
    return 1;
}

int set_hair_colour(string str) {
    hair_colour = str;
    return 1;
}

int set_eyes(string str) {
    eyes = str;
    return 1;
}

int set_eye_colour(string str) {
    eye_colour = str;
    return 1;
}

string query_hair_string() {
    string ret ;

    if(!hair)
        return "no hair" ;

    if(hair && hair_colour)
        ret = replace_string(hair, "$C", hair_colour) ;

    return ret ;
}

string query_eyes_string() {
    string ret ;

    if(!eyes)
        return "no eyes" ;

    if(eyes && eye_colour)
        ret = replace_string(eyes, "$C", eye_colour) ;

    return ret ;
}

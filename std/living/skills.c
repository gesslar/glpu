/**
 * @file /std/living/skills.c
 * @description Trainable skills for living objects
 *
 * @created 2024/07/31 - Gesslar
 * @last_modified 2024/07/31 - Gesslar
 *
 * @history
 * 2024/07/31 - Gesslar - Created
 */

#include <skills.h>
#include <boon.h>

private nomask mapping skills = ([]);

void init_skills() {
    skills = skills || ([]);
    initialize_missing_skills(mud_config("SKILLS"));
}

void wipe_skills() {
    skills = ([]);
}

/**
 * @description Initialize missing skills from the given mapping
 * @param {mapping} skill_set - A mapping of skill names and their details
 */
/**
 * @description Initialize missing skills from the given mapping
 * @param {mapping} skill_set - A mapping of skill categories and skills
 * @param {string} curr_path - The current path in the skill hierarchy
 */
varargs void initialize_missing_skills(mapping skill_set, string curr_path) {
    string cat, skill;
    mixed element;

    if(!curr_path)
        curr_path = "";

    foreach(cat, element in skill_set){
        string full_path = curr_path != "" ? curr_path + "." + cat : cat;

        if(pointerp(element)){
            // Element is an array of skills
            foreach(skill in element){
                string skill_path = full_path + "." + skill;
                add_skill(skill_path, 1.0);
            }
        } else if(mapp(element)){
            // Element is a mapping of further subskills or categories
            initialize_missing_skills(element, full_path);
        } else {
            // Single skill entry (though this case might not occur in this structure)
            add_skill(full_path, 1.0);
        }
    }
}

/**
 * @description Add a skill to the living object
 * @param {string} skill - The name of the skill
 * @param {float} level - The level of the skill with fractional progress
 */

varargs int add_skill(string skill, float level) {
    string *path = explode(skill, ".");
    mapping current = skills;
    int x, sz;

    if(!stringp(skill) || nullp(level) || level < 1.0)
        return null ;

    sz = sizeof(path);
    for(x = 0; x < sz; x++) {
        if(!mapp(current[path[x]])) {
            current[path[x]] = ([
                "level" : (x == sz - 1 ? level : 1.0),
                "subskills" : ([]),
            ]);
        }
        current = current[path[x]]["subskills"];
    }
    return 1;
}

int remove_skill(string skill) {
    string *path = explode(skill, ".");
    mapping current = skills, parent = skills;
    int x, sz;

    if(!stringp(skill))
        return null ;

    sz = sizeof(path);
    for(x = 0; x < sz; x++) {
        if(!mapp(current[path[x]])) return 0;
        if(x == sz - 1) {
            map_delete(parent, path[x]);
            return 1;
        }
        parent = current;
        current = current[path[x]]["subskills"];
    }

    return null ;
}

/**
 * @description Get the level of a skill
 * @param {string} skill - The name of the skill
 * @returns {float} The level of the skill as an integer
 */

float query_skill(string skill) {
    string *path = explode(skill, ".");
    mapping current = skills;
    int x, sz;

    if(!stringp(skill)) return 0.0;

    sz = sizeof(path);
    for(x = 0; x < sz; x++) {
        if(!mapp(current[path[x]])) return 0.0;
        if(x == sz - 1) return current[path[x]]["level"];
        current = current[path[x]]["subskills"];
    }
    return null ;
}

varargs int query_skill_level(string skill, int raw) {
    string *path = explode(skill, ".");
    mapping current = skills;
    int x, sz;

    if(!stringp(skill))
        return null ;

    sz = sizeof(path);
    for(x = 0; x < sz; x++) {
        if(!mapp(current[path[x]]))
            return null ;
        if(x == sz - 1) {
            int lvl = to_int(floor(current[path[x]]["level"]));
            if(raw)
                return lvl;
            else
                return lvl + query_effective_boon("skill", skill) ;
        }

        current = current[path[x]]["subskills"];
    }

    return null ;
}

/**
 * @description Set the level of a skill
 * @param {string} skill - The name of the skill
 * @param {float} level - The level of the skill with fractional progress
 */

int set_skill_level(string skill, float level) {
    string *path = explode(skill, ".");
    mapping current = skills;
    int x, sz;

    if(!stringp(skill) || nullp(level) || level < 1.0)
        return null ;

    sz = sizeof(path);
    for(x = 0; x < sz; x++) {
        if(!mapp(current[path[x]])) return 0;
        if(x == sz - 1) {
            current[path[x]]["level"] = level;
            return 1;
        }
        current = current[path[x]]["subskills"];
    }
    return null ;
}

/**
 * @description Get the skills of the living object
 * @returns {mapping} The skills of the living object
 */

mapping query_skills() {
    return copy(skills);
}

/**
 * @description Set the skills of the living object
 * @param {mapping} skills - The skills of the living object
 */

void set_skills(mapping s) {
    if(!mapp(s)) return;
    skills = copy(s);
}

/**
 * @description Train a skill
 * @param {string} skill - The name of the skill
 * @param {float} progress - The fractional progress to add to the skill level
 */

int train_skill(string skill, float progress) {
    string *path = explode(skill, ".");
    mapping current = skills;
    int x, sz;

    if(!stringp(skill) || nullp(progress) || progress < 0.0)
        return null ;

    sz = sizeof(path);
    for(x = 0; x < sz; x++) {
        if(!mapp(current[path[x]])) return 0;
        if(x == sz - 1) {
            current[path[x]]["level"] += progress;
            return 1;
        }
        current = current[path[x]]["subskills"];
    }

    return null ;
}

int query_skill_progress(string skill) {
    string *path  ;
    mapping current = skills;
    int x, sz;

    if(!stringp(skill))
        return null ;

    path = explode(skill, ".");
    sz = sizeof(path);

    for(x = 0; x < sz; x++) {
        if(!mapp(current[path[x]]))
            return null ;
        if(x == sz - 1)
            return to_int(
                ceil(current[path[x]]["level"]) -
                current[path[x]]["level"]) * 100.0;

        current = current[path[x]]["subskills"];
    }

    return null ;
}

int modify_skill_level(string skill, int level) {
    string *path = explode(skill, ".");
    mapping current = skills;
    int x, sz;

    if(!stringp(skill) || nullp(level))
        return null ;

    sz = sizeof(path);
    for(x = 0; x < sz; x++) {
        if(!mapp(current[path[x]]))
            return 0;
        if(x == sz - 1) {
            current[path[x]]["level"] = level;
            return 1;
        }
        current = current[path[x]]["subskills"];
    }

    return null ;
}

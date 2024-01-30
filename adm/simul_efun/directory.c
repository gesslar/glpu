#include "/adm/obj/simul_efun.h"

// mudlib: basis
// date:   1992/09/05
// author: Truilkan
void mkdirs(string path) {
    string *parts, dir;
    int j;

    parts = explode(path, "/");
    dir = "";
    for (j = 0; j < sizeof(parts); j++) {
        dir += parts[j];
        mkdir(dir);
        dir += "/";
    }
}

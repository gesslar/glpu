#include "/adm/obj/simul_efun.h"

//object.c

//Tacitus @ LPUniversity
//Grouped on October 22nd, 2005

int getoid(object ob) {
    int id;
    sscanf(file_name(ob || previous_object()), "%*s#%d", id);
    return id;
}

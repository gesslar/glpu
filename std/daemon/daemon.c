// /std/daemon/daemon.c
// Base daemon inheritible
//
// Created:     2022/08/23: Gesslar
// Last Change: 2022/08/23: Gesslar
//
// 2022/08/23: Gesslar - Created

inherit __DIR__ "clean" ;

// Functions
void remove() ;

protected void create()
{
    if(function_exists("setup")) call_other("setup", this_object()) ;
}

void remove()
{
    destruct(this_object()) ;
}

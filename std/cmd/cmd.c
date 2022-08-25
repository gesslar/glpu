// /std/cmd/cmd.c
// Base command object
//
// Created:     2022/08/24: Gesslar
// Last Change: 2022/08/24: Gesslar
//
// 2022/08/24: Gesslar - Created

inherit M_CLEAN ;

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

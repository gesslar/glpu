// /std/cmd/skill.c
// Standard skill inheritance for commands
//
// Created:     2024/02/20: Gesslar
// Last Change: 2024/02/20: Gesslar
//
// 2024/02/20: Gesslar - Created

inherit STD_ACT ;

// Functions
int skill_check(object tp, object room, string arg) ;
mixed use(object tp, object room, string arg) {}

// Variables
protected string skill_type ;

void mudlib_setup() {
    skill_type = "skill";
}

mixed main(object tp, object room, string arg) {
    int check_result ;

    // Do pre checks
    check_result = skill_check(tp, room, arg) ;
    if(check_result == 0) return 0 ; // Failure/unavailable/etc
    if(check_result == 2) return 1 ; // Failure, but message already sent

    // Now use
    return use(tp, room, arg);
}

int skill_check(object tp, object room, string arg) {
    return 1;
}

string query_skill_type() {
    return skill_type;
}

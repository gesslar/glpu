// /std/user/tell.c
// Tell lfun
//
// Created:     2022/08/24: Gesslar
// Last Change: 2022/08/24: Gesslar
//
// 2022/08/24: Gesslar - Created

void tell(string message, string cl: (: "tell_player" :)) {
    message(cl, message, this_object()) ;
}

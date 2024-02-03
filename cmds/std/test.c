inherit STD_CMD ;

mixed main(object tp, object room, string arg) {
    tell(tp, "\e<0202>You are in the " + get_short(room) + ".\e<res>\n") ;
    return 1 ;
}

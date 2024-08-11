// save.c

// Gwegster @ LPUniversity
// 30-JUNE-2005
// A simple save command

//Last edited on October 8th, 2005 by Tacitus

inherit STD_CMD ;

mixed main(object caller, string args) {
    caller->save_body();
    write("Successful [save]: User saved.\n");
    return 1;
}

string help(object caller) {
    return(" SYNTAX: save\n\n" +
    "This command will save your charachter data.\n\n" +
    "See also: quit\n");
}

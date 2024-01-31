// /cmds/adm/showtree.c
// Command to show all inheritables of a given object
// Recursively showing as an inheritance tree
//
// Created:     ????/??/??: ?????
// Last Change: 2024/01/31: Gesslar
//
// ????/??/??: Gesslar - ?????

inherit STD_CMD ;

private string dig (string file, string func, int index);

private nosave string Error;
private nosave string FAIL =
"Syntax: showtree <file>\n"
"        showtree <func> in <file>\n" ;

void setup() {
    help =
"Syntax: showtree <file>\n"
"        showtree <func> in <file>\n"
"\n"
"In its first version, shows you the full inheritance tree for the named "
"object. In its second form, it shows you all files in the inheritance tree "
"which contain the function you name, specifically noting those objects which "
"have definitions for the function."
;
}

mixed main(object tp, object room, string str) {
    string func, file, result;
    object ob ;

    if(!str)
        return FAIL;

    if(sscanf(str, "%s in %s", func, file) != 2 && sscanf(str, "%s %s", func, file) != 2) {
        func = 0;
        file = str;
    }

    ob = get_object(file) ;
    if(ob) file = base_name(ob);
    else file = resolve_path(tp->query("cwd"), file);

    result = dig(file, func, 0);

    if(!result) {
        tp->tell(Error);
        return 1;
    }

    return result ;
}

string dig (string file, string func, int indent) {
    object ob;
    string str;
    int found;

    if( strlen(file) > 2 && file[<2..] == ".c" ) file = file[0..<3];
    if( file[0] != '/' ) file = "/" + file;
    if(!file_exists(file+".c")) {
        Error = "No such file " + file + ".c\n";
        return 0;
    }

    if( !(ob = load_object(file)) ) {
        Error = "Error in loading " + file + ".\n";
        return 0;
    }

    str = sprintf("%*-' 's" + file, indent*2, "");
    if(func && (function_exists(func, ob) == file) ) {
        str += " (\e<0078>" + func + "\e<res> defined)\n";
    } else {
        str += "\n";
    }

    if( !func || function_exists(func, ob) ) found = 1;
    indent++;
    foreach(file in sort_array(inherit_list(ob), 1)) {
        string tmp;
        tmp = dig(file, func, indent);
        if( tmp && tmp != "" ) {
            str += tmp;
            found = 1;
        } else if( !tmp ) {
            return 0;
        }
    }
    return (found ? str : "");
}

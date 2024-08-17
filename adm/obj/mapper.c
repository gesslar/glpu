/**
 * @file /adm/obj/mapper.c
 * @description Object to map area exits
 *
 * @created 2024-08-16 - Gesslar
 * @last_modified 2024-08-16 - Gesslar
 *
 * @history
 * 2024-08-16 - Gesslar - Created
 */

inherit STD_ITEM ;

string syntax();
string reversie(string dir);

mapping *map = ({ }) ;

void setup() {
    set("short", "@@query_short");
    set("long", syntax());
    set("prevent_get", 1);
    set("prevent_drop", 1);
    set_id(({ "area exit mapper", "mapper", "Area Exit Mapper" }));
    set_name("area exit mapper");
}

string syntax() {
return (@text

`<fbb>`Area Mapper`<res>`

This object will attempt to find any exits that don't point
back to the originating room. It will travel a range of rooms
and report back to its master its findings.

 To start:   `<fbb>`maparea [-vq]`<res>`
  To stop:   `<fbb>`maparea stop`<res>`

Options: v = Verbose. Show all output. Default is to only show errors.
         q = Quiet. Do not beep when errors are found. Default is to beep.

text
);
//'
}

void init() {
    add_action("maparea", "maparea");
}

int maparea(string str)
{
    object tp = this_player() ;
    string mess;

    if(str)
    {
        if(query("testing"))
        {
            if(str != "stop")
            {
                tell_object(tp, "The Area Exit Mapper is already running.\n");
                return 1;
            }

            delete("testing");
            tell_object(tp, "Testing stopped.\n");
            return 1;
        }
        else
        {
            if(str[0..0] == "-")
            {
                str = extract(str, 1);
                if(strsrch(str, "v") != -1)
                set("verbose", 1);
                if(strsrch(str, "q") != -1)
                set("quiet", 1);
            }
            else if (str == "stop")
            {
                tell_object(tp, "The Area Exit mapper is already running.\n");
                return 1;
            }
            else
            {
                write(syntax());
                return 1;
            }
        }
    }
    else
    {
        if(query("testing"))
        {
            tell_object(tp, "The Area Exit Mapper is already running.\n");
            return 1;
        }
    }

    mess = "What is the \"base\" filename of the files in your "
            "area \n(ex: /d/domain/areaXXX.c the base would be: /d/domain/area): ";
    tell_object(tp, mess);
    input_to("get_area", tp);
    return 1;
}

void get_area(string area, object tp)
{
    string mess;
    if(!area || !sizeof(area))
    {
        tell_object(tp, "\nCancelled.\n");
        return ;
    }

    mess = "\nIf you do not pad the digits in your file names with "
        "zeros\ntype 0 (zero), else, type the number of columns of digits "
        "you\nexpect in your area file names: ";
    tell_object(tp, mess);
    input_to("get_nums", area, tp);
}

void get_nums(string str, string area, object tp)
{
    int n;

    if(sscanf(str, "%d", n) != 1)
    {
        tell_object(tp, "\nCancelled!\n");
        return ;
    }

    set("base", area);
    set("num", n);

    str = sprintf("\n%s: %s\n%s: %d\n",
        "`<fbb>`Area`<res>`",
        area,
        "`<fbb>`Nums`<res>`",
        n);

    tell_object(tp, str);
    tell_object(tp, "\nStarting in 1 second...\n\n");
    set("testing", 1);
    call_out("process_room", 1, 1, tp, 0, 0);
}

string query_short()
{
    if(query("testing")) return "`<fbb>`Area Exit Mapper`<res>` (testing)";
    else return "`<fbb>`Area Exit Mapper`<res>`";
}

void process_room(int num, object tp, int x, int y)
{
    mapping exits;
    string *dirs, roomname, dir;
    string file, printer, dest_file;
    object room, dest;

    if(!query("testing")) return ;

    if(query("num") != 0)
        printer = query("base")+"%:"+sprintf("%d", query("num"))+"d";
    else
        printer = query("base")+"%d";

    file = sprintf(printer,  num);

    if(!room = load_object(file))
    {
        tell_object(tp, "Could not load " + file + "\nStopping.");
        delete("testing");
        return ;
    }

    exits = room->query_exits();
    dirs = keys(exits);
    tell_object(tp, "Testing: " + room->query("short") + " ("+file+")\n");
    if(!sizeof(exits))
    {
        tell_object(tp, "There are no exits for this room.\n");
        call_out_walltime("process_room", 0.5, num+1, tp);
        return ;
    }

    foreach(dir, dest_file in exits)
    {
        string out;
        int fail;

        fail = 0;
        out = "  => Checking " + dir + "<->" + reversie(dir) + " ";

        if(!file_exists(dest_file+".c"))
        {
            fail = 1;
            out += "`<fbb>`Skipping!`<res>`\n";
            continue;
        }
        else
        {
            dest = load_object(dest_file);
            roomname = dest->query_file_name();
            out += "(`<fad>`"+roomname+"`<res>`) ";
            dir = reversie(dir);
            if(dest->query("exits")[dir] == file)
            {
                out += "`<cfb>`OK`<res>`\n";
            }
            else
            {
                fail = 1;
                if(query("quiet")) out += "`<fac>`FAIL`<res>` `<daf>`("+dir+")`<res>`\n";
                else out += "\a`<fac>`FAIL`<res>` `<daf>`("+dir+")`<res>`\n";
            }
        }
        if(fail) tell_object(tp, out);
        if(!fail && query("verbose")) tell_object(tp, out);
    }
    call_out_walltime("process_room", 0.1, num+1, tp);
}

string reversie(string dir)
{
    switch(dir)
    {
        case "north": return "south";
        case "east": return "west";
        case "south": return "north";
        case "west": return "east";
        case "northeast": return "southwest";
        case "northwest": return "southeast";
        case "southeast": return "northwest";
        case "southwest": return "northeast";
        case "up": return "down";
        case "down": return "up";
        default: return "(UNMATCHED)";
    }
}

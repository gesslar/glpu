/* master.c

 Tacitus @ LPUniversity
 April 15th, 2005
 master object

 Last edited on July 14th, 2006 by Tacitus

*/

/* Preprocessor Statement */

#include <config.h>
#include <localtime.h>

#pragma show_error_context

/* inherits */

inherit "/adm/obj/master/valid";

/* Functions */

void flag(string str) {
    debug_message("Flags disabled.\n");
}

protected object connect(int port) {
    object login_ob;
    mixed err;

    err = catch(login_ob = new(LOGIN_OB));

    if (err) {
    write("I'm sorry, but it appears that mud is not functional at the moment.\n");
    write(err);
    destruct(this_object());
    }
    return login_ob;
}

protected void epilog(int load_empty) {
    string str, *lines, err;
    int i, time;
    string out = "" ;

    set_privs(this_object(), "[master]");
    lines = explode_file("/adm/etc/preload");

    if(!sizeof(lines)) return;

    for (i = 0; i < sizeof(lines); i++) {
        out = "" ;
        out += "Preloading : " + lines[i] + "..." ;
        err = catch(load_object(lines[i]));

        if (err != 0) {
            out += "\nError " + err + " when loading " + lines[i];
        } else {
            time = time() - time;
            out += "Done (" + time/60 + "." + time % 60 + ")";
        }

        debug_message(out);
    }
}

protected void log_error(string file, string message) {
    string username;

    if(this_player()) username = this_player()->query_name();
    if(stringp(username)) {
        string path = user_path(username);
        if(directory_exists(path)) {
            write_file(path + "log", "\n" + message);
            if(this_player()->query_env("error_output") != "disabled")
                tell_object(this_player(), message);
        }
    }

    log_file("compile", message);
}

//error_handler needs to be rewritten
protected void error_handler(mapping map, int flag) {
    object ob;
    string logContent = "---\n";

    ob = this_interactive() || this_player();
    if(flag) logContent += "**Error Caught\n";
    logContent += sprintf("Error: %s\nCurrent Object: %-20O\nCurrent Program: %-20O\nFile: %-20O Line: %-20d\n\nTraceback:\n%O\n",
      map["error"], (map["object"] || "No current object"), (map["program"] || "No current program"),
      map["file"], map["line"],
      implode(map_array(map["trace"],
      (: sprintf("Line: %O  File: %O \nObject: %O\n Program: %O", $1["line"], $1["file"], $1["object"] || "No object", $1["program"] ||
          "No program") :)), "\n"));

    if(ob) {
        if(sscanf(map["file"], "/home/%*s/" + ob->query_name() + "/%*s"))
            write_file(user_path(ob->query_name()) + "log", logContent);
        if(!flag) tell_object(ob, logContent);
    }

    log_file("log", logContent);
}

protected void crash(string crash_message, object command_giver, object current_object) {
    foreach (object ob in users()) {
        tell_object(ob, "Master object shouts: Damn!\nMaster object tells you: The game is crashing.\n");
        catch(ob->save_user());
    }

    log_file("shutdown", MUD_NAME + " crashed on: " + ctime(time()) +
        ", error: " + crash_message + "\n");

    log_file("crashes", MUD_NAME + " crashed on: " + ctime(time()) +
        ", error: " + crash_message + "\n");

    if (command_giver) {
        log_file("crashes", "this_player: " + file_name(command_giver) + " :: " + command_giver->query_name() + "\n");
    }

    if (current_object) {
        log_file("crashes", "this_object: " + file_name(current_object) + "\n");
    }

}

string get_save_file_name(string file, object who)
{
    return "/tmp/ed_SAVE_" + who->query_name() + "#" + file + random(1000);
}

string privs_file(string filename) {
    string temp;
    if(sscanf(filename, "/adm/daemons/%s", temp)) return "[daemon]";
    if(sscanf(filename, "/adm/obj/%s", temp)) return "[adm_obj]";
    if(sscanf(filename, "/adm/%s", temp)) return "[admin]";
    if(sscanf(filename, "/cmds/adm/%s", temp)) return "[cmd_admin]";
    if(sscanf(filename, "/cmds/file/%s", temp)) return "[cmd_file]";
    if(sscanf(filename, "/cmds/object/%s", temp)) return "[cmd_object]";
    if(sscanf(filename, "/cmds/wiz/%s", temp))return "[cmd_wiz]";
    if(sscanf(filename, "/cmds/%s", temp)) return "[cmd]";
    if(sscanf(filename, "/home/%*s/%s/%*s", temp)) return "[home_" + temp + "]";
    if(sscanf(filename, "/open/%s", temp)) return "[open]";
    if(sscanf(filename, "/std/%s", temp)) return "[std_object]";
    if(sscanf(filename, "/obj/mudlib/%s", temp)) return "[mudlib_object]";
    if(sscanf(filename, "/obj/%s", temp)) return "[gen_obj]";
    else return "object";
}

string object_name(object ob) {
    if(ob->query_name()) return ob->query_name();
    return file_name(ob);
}

mixed compile_object(string file) {
    string *pathExploded, serverPath;
    object server, virtualObject;
    int index;

    write_file("/log/virtual_compile", "Request: " + file + " ("+previous_object()+")\n") ;

    pathExploded = explode(file, "/");
    index = member_array("virtual_area", pathExploded);

    if(index != -1) {
        serverPath = "/" + implode(pathExploded[0..index], "/") + "/virtual_server.c";
        if(file_exists(serverPath)) {
            write_file("/log/virtual_compile", "Server: " + serverPath + " ("+previous_object()+")\n") ;

            catch(server = load_object(serverPath));
            if(objectp(server)) {
                virtualObject = server->compile_object(file);

                if(objectp(virtualObject))  {
                    write_file("/log/virtual_compile", "Virtual object loaded for " + file + " ("+previous_object()+")\n") ;
                    return virtualObject;
                } else {
                    write_file("/log/virtual_compile", "No Virtual object loaded for " + file + " ("+previous_object()+")\n") ;
                    return 0;
                }
            }
        }

        write("No functional server found at " + serverPath + " ("+previous_object()+")\n") ;
    }

    return 0;
}

string make_path_absolute(string file) {
    file = resolve_path((string)this_player()->query_cwd(), file);
    return file;
}

void log_file(string file, string msg) {
    int size;

    if(query_privs(previous_object()) == "[open]") return;

    size = file_size(log_dir() + file);
    if(size == -2) return;
    if(size > 50000) {
        mixed *localtime_now = localtime(time());
        string t1;
        string backup;
        int ret = sscanf(file, "%s.log", t1);

        if(ret == 0)
            backup  =
                sprintf("archive/%s-%02d%02d%02d%02d",
                    file,
                    localtime_now[LT_MON],
                    localtime_now[LT_MDAY],
                    localtime_now[LT_HOUR],
                    localtime_now[LT_MIN],
                );
        else
            backup  =
                sprintf("archive/%s-%02d%02d%02d%02d.log",
                    t1,
                    localtime_now[LT_MON],
                    localtime_now[LT_MDAY],
                    localtime_now[LT_HOUR],
                    localtime_now[LT_MIN],
                );
            rename(log_dir() + file, log_dir() + backup);
    }

    write_file(log_dir() + file, msg);
}

int save_ed_setup(object user, mixed config) {
    user->set("ed_setup", config);
    return 1;
}

int retrieve_ed_setup(object user) {
    return user->query("ed_setup");
}

mapping get_mud_stats() {
    return MSSP_D->get_mud_stats();
}

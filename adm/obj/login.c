/*login.c

* Tacitus @ LPUniversity
* 04-APR-05
* Login Daemon

* Recoded on October 7th, 2005 by Tacitus

*/

/* Last edited on October 6th, 2006 by Tacitus */

#include <logs.h>
#include <daemons.h>
#include <origin.h>
#include <rooms.h>

inherit STD_ITEM ;

inherit M_MESSAGING ;
inherit M_GMCP ;
inherit M_LOG ;

private nosave mapping login_gmcp_data = ([
    "client" : null,
    "supports" : null,
]) ;

private nosave mapping environ_data = ([]) ;

/* #define EMAIL_MUST_RESOLVE */

void get_name(string str);
void get_password(string str, int i);
void verify_password(string str, int i);
void new_user(string str, string name);
void auto_destruct();
void reconnect(string str);
void setup_new();
void enter_world(int reconnecting);
void email_verified(string address, string resolved, int key);
void get_email(string input);
void idle_email(string str);
object create_body(string name);
object create_user() ;
string parse_tokens(string text);

object user;
object body;

string motd = "\n" + parse_tokens(read_file(mud_config("LOGIN_NEWS"))) ;
string login_message = parse_tokens(read_file(mud_config("LOGIN_MSG")));
string email;

int is_connected = 0, call_out_id;

void create() {
    set_log_level(0) ;

    if(clonep()) call_out_id = call_out("auto_destruct", 60);
}

void logon() {
    tell(this_object(), login_message) ;
    tell(this_object(), "\nPlease select a name: ", MSG_PROMPT) ;
    input_to("get_name");
}

void get_name(string str) {
    int i;

    load_object(LOCKDOWN_D);

    if(LOCKDOWN_D->is_ip_banned(query_ip_number(this_object()))) {
        tell(this_object(),"\nYour IP address, " + query_ip_number(this_object()) + " has been banned from " + mud_name() + ".\n");
        remove() ;
        return;
    }

    if(!str || strlen(str) < 2) {
        tell(this_object(),"You must select a name: ");
        input_to("get_name");
        return;
    }

    if(str == "quit") {
        tell(this_object(),"Come back soon!\n");
        remove() ;
        return;
    }

    if(strlen(str) > 11) {
        tell(this_object(),"Sorry, your name can't have more than 11 characters.\n");
        tell(this_object(),"Please select a name: ");
        input_to("get_name");
        return;
    }

    str = lower_case(str);

    for (i = 0; i < strlen(str); i++) {
        if(str[i] < 'a' || str[i] > 'z') {
            tell(this_object(),"Sorry, your name can only have letters. (a-z)\n"
                "Please enter a new name: ");
            input_to("get_name");
            return;
        }
    }

    if(LOCKDOWN_D->is_user_banned(str)) {
        tell(this_object(),"\nThe username " + capitalize(str) + " is banned from " + mud_name() + ".\n");
        remove() ;
        return;
    }

    if(LOCKDOWN_D->query_dev_lock() && wizardp(str) && !adminp(str)) {
        tell(this_object(),"\n" + LOCKDOWN_D->query_dev_lock_msg() + "\n");
        remove() ;
        return;
    }

    if(LOCKDOWN_D->query_player_lock() && (!adminp(str) && !wizardp(str))) {
        tell(this_object(),"\n" + LOCKDOWN_D->query_player_lock_msg() + "\n");
        remove() ;
        return;
    }

    if(LOCKDOWN_D->query_vip_lock() && (!adminp(str) && !wizardp(str) && (member_array(str, LOCKDOWN_D->query_play_testers()) == -1))) {
        tell(this_object(),"\n" + LOCKDOWN_D->query_vip_lock_msg() + "\n");
        remove() ;
        return;
    }

    if(str == "guest") {
        if(LOCKDOWN_D->query_guest_locked()) {
            tell(this_object(),"\n" + LOCKDOWN_D->query_guest_lock_msg() + "\n");
            remove() ;
            return;
        }

        if(!user = create_user())
            return ;

        user->set_name("guest");
        set_privs(user, "guest");
        body = create_body("guest");
        write_file(log_dir() + LOG_LOGIN, capitalize(user->query_proper_name()) + " ("+getoid(body)+") logged in from " +
          query_ip_number(this_object()) + " on " + ctime(time()) + "\n");
        if(mud_config("DISPLAY_NEWS")) {
            tell(this_object(), motd + "\n");
            tell(this_object()," [Hit enter to continue] ");
            input_to("enter_world", 0);
        } else {
            enter_world(0);
        }
        return;
    }

    if(!file_exists(user_data_file(str) + ".o")) {
        if(LOCKDOWN_D->query_player_lock()) {
            tell(this_object(),"\n" + LOCKDOWN_D->query_player_lock_msg() + "\n");
            remove() ;
            return;
        }

        tell(this_object(),"The user " + str + " does not exist. Would you like to create it? ");
        input_to("new_user", str);
        return;
    }

    if(!body = find_player(str)) {
        if(!user = create_user())
            return ;
        else {
            set_privs(user, str);
            user->set_name(str);
            user->restore_user();
        }
    } else {
        if(!user = body->query_user()) {
            if(!user = create_user())
                return ;
            else {
                set_privs(user, str);
                user->set_name(str);
                user->restore_user();
            }
        }
    }

    if(!user->query_password() || user->query_password() == "") {
        tell(this_object(),"Your account has no password. All accounts must have a password.\n");
        tell(this_object(),"Please enter a new password: ");
        input_to("get_password", 1, 2);
        return;
    }

    tell(this_object(),"Please enter your password: ");
    input_to("get_password", 1, 0);
}

void get_password(string str, int i) {
    string pass;
    string curr ;

    if(!i) {
        curr = user->query_password() ;
        pass = crypt(str, curr);
        if(pass != curr) {
            tell(this_object(),"That password is incorrect.\n");
            tell(this_object(),"Please enter your password: ");
            input_to("get_password", 1, 0);
            return;
        } else {
            is_connected = 1;
            if(find_player(user->query_proper_name())) {
                tell(this_object(),"\nReconnect to currently logged in body? ");
                input_to("reconnect");
                return;
            }
            if(!body)
                body = create_body(query_privs(user));
            write_file(log_dir() + "/" + LOG_LOGIN, capitalize(user->query_proper_name()) + " ("+getoid(body)+") logged in from " +
              query_ip_number(this_object()) + " on " + ctime(time()) + "\n");
            if(mud_config("DISPLAY_NEWS")) {
                tell(this_object(), motd + "\n");
                tell(this_object()," [Hit enter to continue] ");
                input_to("enter_world", 0);
            } else {
                enter_world(0);
            }
            return;
        }
    } else {
        if(!str) {
            tell(this_object(),"Your user must have a password.\n");
            tell(this_object(),"Please enter a password: ");
            input_to("get_password", 1, 0);
            return;
        }

        user->set_password(str);
        tell(this_object(),"\nPlease enter your password again to verify: ");
        input_to("verify_password", 1, i);
        return;
    }
}

void verify_password(string str, int i) {
    string curr ;

    curr = user->query_password() ;
    str = crypt(str, curr) ;
    if(str != user->query_password()) {
        tell(this_object(),"Your passwords do not match.\n");
        tell(this_object(),"Please enter a password: ");
        input_to("get_password", 1, 1);
        return;
    } else if(i != 2) {
        tell(this_object(),"\nPlease input your public e-mail address: ");
        input_to("get_email");
        return;
    } else {
        tell(this_object(),"\nYou may now login with the password you just set.\n");
        tell(this_object(),"Please enter your password: ");
        input_to("get_password", 1, 0);
        return;
    }
}

void get_email(string input) {
    string host;

    if(!input) {
        tell(this_object(),"Error [login]: You must provide an e-mail address.\n");
        tell(this_object(),"Please input your e-mail address: ");
        input_to("get_email");
        return;
    }

    if(!sscanf(input, "%*s@%s", host)) {
        tell(this_object(),"Error [login]: You must provide a valid e-mail address.\n");
        tell(this_object(),"Please input your e-mail address: ");
        input_to("get_email");
        return;
    }

    if(host == "localhost") {
        tell(this_object(),"Error [login]: Please provide a different e-mail address.\n");
        tell(this_object(),"Please input your e-mail address: ");
        input_to("get_email");
        return;
    }

    email = input;

    resolve(host, "email_verified");
    tell(this_object(),"\n Please hit any key to continue.\n");
    input_to("idle_email");
}

void idle_email(string str) {
    object security_editor;

#ifdef EMAIL_MUST_RESOLVE
    if(!is_connected) {
        tell(this_object(),"Error [login]: Unable to verify e-mail. Press press any key to try again.\n");
        input_to("idle_email");
        return;
    } else {
#endif
        set_privs(user, user->query_proper_name());
        if(!body)
            body = create_body(query_privs(user));

        if(!file_exists(mud_config("FIRST_USER"))) {
            if(!directory_exists("/home/" + user->query_proper_name()[0..0])) mkdir ("/home/" + user->query_proper_name()[0..0]);
            mkdir("/home/" + user->query_proper_name()[0..0] + "/" + user->query_proper_name());
            mkdir("/home/" + user->query_proper_name()[0..0] + "/" + user->query_proper_name() + "/public");
            mkdir("/home/" + user->query_proper_name()[0..0] + "/" + user->query_proper_name() + "/private");
            catch(cp("/d/std/workroom.c", user_path(user->query_proper_name())));
            write_file("/home/" + user->query_proper_name()[0..0] + "/" + user->query_proper_name() + "/private/journal." + user->query_proper_name(), capitalize(user->query_proper_name()) + "'s dev journal (Created: " + ctime(time()) + ")\n\n");
            catch(link("/home/" + user->query_proper_name()[0..0] + "/" + user->query_proper_name() + "/private/journal." + user->query_proper_name(), "/doc/journals/journal." + user->query_proper_name()));
            body->add_path("/cmds/wiz/");
            body->add_path("/cmds/object/");
            body->add_path("/cmds/file/");
            body->add_path("/cmds/adm/");
            security_editor = new(OBJ_SECURITY_EDITOR);
            security_editor->enable_membership(query_privs(user), "developer");
            security_editor->enable_membership(query_privs(user), "admin");
            security_editor->write_state(0);
            write_file(mud_config("FIRST_USER"), user->query_proper_name(), 1) ;
            tell(this_object(),"Success [login]: You are now an admin.\n\n");
        }

        user->set("email", email);
        write_file(log_dir() + LOG_NEWUSER, capitalize(user->query_proper_name()) + " created a new account from "
          + query_ip_number(this_object()) + " on " + ctime(time()) + "\n");
        write_file(log_dir() + LOG_LOGIN, capitalize(user->query_proper_name()) + " ("+getoid(body)+") logged in from " +
          query_ip_number(this_object()) + " on " + ctime(time()) + " for the first time.\n");
        tell(this_object(),"\n" + parse_tokens(read_file(mud_config("FLOGIN_NEWS"))) + "\n");
        tell(this_object()," [Hit enter to continue] ");
        input_to("setup_new");
#ifdef EMAIL_MUST_RESOLVE
    }
#endif
}

void email_verified(string address, string resolved, int key)
{
#ifdef EMAIL_MUST_RESOLVE
    if(!resolved) {
        tell(this_object()," Error [login]: Unable to verify e-mail.\n");
        tell(this_object(),"Please input your e-mail address: ");
        input_to("get_email");
        return;
    }
#endif
    is_connected = 1;
}

void new_user(string str, string name) {
    if(str == "y" || str == "yes" || str == "yup" || str == "sure" || str == "indeed") {
        user = create_user() ;
        user->set_name(name);
        tell(this_object(),"Please enter a password for your account: ");
        input_to("get_password", 1, 1);
        return;
    }

    tell(this_object(),"Please select another name then: ");
    input_to("get_name");
}

void setup_new() {
    if(!directory_exists(user_data_directory(user->query_proper_name())))
        mkdir(user_data_directory(user->query_proper_name()));
    user->save_user();
    if(objectp(body)) body->save_user();
    if(mud_config("DISPLAY_NEWS")) {
        tell(this_object(), motd + "\n");
        tell(this_object(),"\n");
        tell(this_object()," [Hit enter to continue] ");
        input_to("enter_world", 0);
    } else {
        enter_world(0);
    }
}

void reconnect(string str) {
    object old_body = find_player(user->query_proper_name());

    if(str == "y" || str == "yes" || str == "yup" || str == "sure" || str == "indeed") {
        tell_object(old_body, "Warning: Your body has been displaced by another user.\n");
        write_file(log_dir() + LOG_LOGIN, capitalize(old_body->query_proper_name()) + " ("+getoid(old_body)+") reconnected from " +
          query_ip_number(this_object()) + " on " + ctime(time()) + "\n");
        if(interactive(old_body)) remove_interactive(old_body);
        old_body->reconnect();
        body = old_body;
        if(mud_config("DISPLAY_NEWS")) {
            tell(this_object(), motd + "\n");
            tell(this_object()," [Hit enter to continue] ");
            input_to("enter_world", 1);
        } else {
            enter_world(1);
        }
        return;
    } else {
        body->remove() ;
        tell(this_object(), "You have chosen not to reconnect to your old body.\n");
        write_file(log_dir() + LOG_LOGIN, capitalize(user->query_proper_name()) + " ("+getoid(body)+") logged in from " +
          query_ip_number(this_object()) + " on " + ctime(time()) + "\n");
        if(mud_config("DISPLAY_NEWS")) {
            tell(this_object(), motd + "\n");
            tell(this_object()," [Hit enter to continue] ");
            input_to("enter_world", 0);
        } else {
            enter_world(0);
        }
        return;
    }
}

void auto_destruct() {
    if(is_connected) return;
    if(interactive(this_object())) tell(this_object(),"\n\nNotice: I'm sorry, but you are only allowed 60 seconds to attempt to login.\n");
    remove() ;
}

void enter_world(int reconnecting) {
    string loc ;
    string e ;
    object room ;

    if(!objectp(body))
        body = BODY_D->create_body(user) ;

    if(body->is_dead()) {
        body->remove() ;
        body = BODY_D->create_ghost(user);
    }

    exec(body, this_object());
    body->setup_body() ;
    body->set_user(user);
    user->clear_gmcp_data() ;
    user->set_gmcp_data(login_gmcp_data) ;
    user->clear_environ_data() ;
    user->set_environ_data(environ_data) ;
    user->set_body(body);

    if(body->gmcp_enabled())
        GMCP_D->init_gmcp(body) ;

    // Now let's figure out where to put them
    if(devp(user)) {
        loc = body->query_env("start_location") ;
        if(loc == "last_location")
            loc = body->query("last_location") ;
        else
            loc = user_path(body) + "workroom" ;
    } else {
        loc = ROOM_START ;
    }

    loc = append(loc, ".c") ;

    if(!file_exists(loc)) {
        tell(this_object(),"Error [login]: Unable to find your start location.\n");
        tell(this_object(),"Please contact an admin for assistance.\n");
    }

    if(e = catch(room = load_object(loc))) {
        tell(this_object(),"Error [login]: Unable to load your start location\n");
        tell(this_object(),"Please contact an admin for assistance.\n");
    }

    if(e = catch(body->move_living(room, null, null, "SILENT"))) {
        tell(this_object(),"Error [login]: Unable to move you to your start location.\n");
        tell(this_object(),"Please contact an admin for assistance.\n");
    }

    if(devp(user) && !environment(body)) {
        // Try one more time to put them in the start location
        loc = ROOM_START ;
        if(e = catch(room = load_object(loc))) {
            tell(this_object(),"Error [login]: Unable to load your start location\n");
            tell(this_object(),"Please contact an admin for assistance.\n");
        }

        if(e = catch(body->move_living(room, null, null, "SILENT"))) {
            tell(this_object(),"Error [login]: Unable to move you to your start location.\n");
            tell(this_object(),"Please contact an admin for assistance.\n");
        }
    }

    if(!environment(body)) {
        user->remove() ;
        body->remove() ;
        remove() ;
        return;
    }

    body->enter_world();
    remove_call_out(call_out_id);

    if(reconnecting)
        emit(SIG_USER_LINK_RESTORE, user) ;
    else
        emit(SIG_USER_LOGIN, user) ;

    remove() ;
}

void relogin() {
    object login = new(LOGIN_OB);

    body = this_body();
    user = body->query_user();
    body->exit_world();
    log_file(LOG_LOGIN, capitalize(user->query_proper_name()) + " ("+getoid(body)+") logged out (relogin) from " +
      query_ip_number(body) + " on " + ctime(time()) + "\n");
    exec(login, body);
    body->save_user();
    body->remove();
    login->logon();
}

object create_body(string name) {
    string err;

    if(origin() != ORIGIN_LOCAL) return 0;

    err = catch(body = new(user->query_body_path()));

    if(err) {
        error("Error [login]: There was an error creating your mobile.\n" +
          "\tBody: " + user->query_body_path() + "\n" + err);
    }

    body->set_name(query_privs(user));
    user->set_body(body);
    set_privs(body, body->query_proper_name());
    body->restore_user();

    return body;
}

object create_user() {
    object user;
    mixed err;

    if(origin() != ORIGIN_LOCAL) return 0;

    err = catch(user = new(STD_USER));

    if(err) {
        tell(this_object(),"Error [login]: Unable to create user object.\n");
        tell(this_object(),"It appears that the login object is dysfunctional. Try again later.\n");
        tell(this_object(),"Error: " + err + "\n");
        remove() ;
        return 0 ;
    }

    return user;
}

string query_proper_name() {
    return "login";
}

string parse_tokens(string text) {
    catch {
        text = replace_string(text, "%mud_name", mud_name());
        text = replace_string(text, "%users", implode(
            filter(users()->query_name(), (: $1 != "login" :))[0..<2], ", ") +
            ", and " + filter(users()->query_name(),
            (: $1 != "login" :) )[<1]);
        text = replace_string(text, "%user_count", "" + sizeof(users()));
        text = replace_string(text, "%date", ctime(time()));
        text = replace_string(text, "%open_status", open_status()) ;
        text = replace_string(text, "%email", admin_email()) ;
        text = replace_string(text, "%lib_name", lib_name());
        text = replace_string(text, "%lib_version", lib_version());
        text = replace_string(text, "%baselib_version", baselib_version());
        text = replace_string(text, "%cap_mud_name",
            implode(explode(mud_name(), ""), (: capitalize($1)
            + capitalize($2) :)));
        text = replace_string(text, "%driver_version", driver_version());
    };

    return text;
}

void set_gmcp_client(mapping client) {
    login_gmcp_data["client"] = client ;
}

mapping query_gmcp_client() {
    return login_gmcp_data["client"] ;
}

void set_gmcp_supports(mapping supports) {
    login_gmcp_data["supports"] = supports ;
}

mapping query_gmcp_supports() {
    return login_gmcp_data["supports"] ;
}

object query_user() {
    return user;
}

void receive_environ(string var, mixed value) {
    environ_data[var] = value ;
}

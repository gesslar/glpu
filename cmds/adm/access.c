//access.c

//Tacitus @ LPUniversity
//09-OCT-05
//Advanced admin command

//THIS IS A VERY EARLY VERSION AND ISN'T VERY USERFRIENDLY YET!

/* Last edited on July 17th, 2006 by Tacitus */

inherit STD_CMD ;

#define FILE_GROUPDATA "/adm/etc/groups"
#define FILE_ACCESSDATA "/adm/etc/access"

mapping access = ([]);
mapping groups = ([]);

void write_gFile(int flag);
void write_aFile(int flag);
string *parse(string str);
void parse_files();
void parse_group();
void parse_access();

int i_MainMenu(string str);
int i_ModMenu(string str);
int i_DisMenu(string str);
int i_cnDisMenu(string str);
int i_qGroup(string str);
int i_crGroup(string str);
int i_dlGroup(string str);
int i_tgMembership(string str);
int i_tgMembership2(string str, string user);
int i_mdDirectory(string str);
int i_mdDirGroup(string str, string dir);
int i_mdDirAKey(string str, string dir, string group);

void setup() {
     parse_files();
}

mixed main(object tp, string arg) {
     if(!adminp(this_interactive())) return notify_fail("Error [access]: Access denied.\n");
     write("\n\t--'Security System Interface'--\n"
           "\t\tVersion: 1.3RCDev\n\n");
     write(" 1: Display current access settings\n");
     write(" 2: Modify current access settings\n");
     write(" 3: Reload data from file\n");
     write(" 4: Save & Exit\n");
     write(" 5: Exit without saving\n");
     input_to("i_MainMenu", tp);
     return 1;
}

int i_MainMenu(string str, object tp) {
     if(!str) return 1;
     switch(str) {
          case "1" : {
               write("\n\tDISPLAY CURRENT ACCESS SETTINGS\n\n");
               write(" 1: Display raw data\n");
               write(" 2: List all groups\n");
               write(" 3: List members of a group\n");
               write(" 4: List access for directory\n");
               write(" 5: Main menu\n");
               input_to("i_DisMenu", tp);
               return 1;
          }
          case "2" : {
               write("\n\tMODIFY CURRENT ACCESS SETTINGS\n\n");
               write(" 1: Create group\n");
               write(" 2: Delete group\n");
               write(" 3: Toggle user membership to group\n");
               write(" 4: Modify access to a directory\n");
               write(" 5: Main menu\n");
               input_to("i_ModMenu", tp);
               return 1;
          }
          case "3" : {
               parse_files();
               write("\nSuccess: Data reloaded from file -- All unsaved changes lost.\n");
               main(tp, 0, 0);
               return 1;
          }

          case "4" : {
               write("Attempting to write data to files...\n");
               write_gFile(0);
               write_aFile(0);
               parse_files();
               write("Success [access]: Exited with data saved.\n");
               return 1;
          }
          case "5" : {
               write("Success [access]: Exiting without saving.\n");
               parse_files();
               return 1;
          }
          default : {
               write("Error [access]: Unknown menu " + str + "\n");
               return 1;
          }
     }
}

int i_ModMenu(string str, object tp) {
     string *arr;

     arr = ({});

     switch(str) {
          case "1" : {
               write("Enter the name of the group you wish to create: ");
               input_to("i_crGroup", tp);
               return 1;
          }

          case "2" : {
               write("Enter the name of the group you wish to delete: ");
               input_to("i_dlGroup", tp);
               return 1;
          }

          case "3" : {
               write("Enter the name of the user who you wish to toggle membership: ");
               input_to("i_tgMembership", tp);
               return 1;
          }

          case "4" : {
               write("Enter the directory that you wish to modify the access to: ");
               input_to("i_mdDirectory", tp);
               return 1;

          }

          default : {
               write("\n");
               main(tp, 0, 0);
               return 1;
          }

     }

}

int i_mdDirectory(string str, object tp) {
     if(!str) {
          write("\n");
          i_MainMenu("2", tp);
          return 1;
     }

     if(str[<1..<1] != "/") str += "/";

     write("\nEnter the name of the group/user whom's access you wish to edit: ");
     input_to("i_mdDirGroup", tp, str);
     return 1;
}

int i_mdDirGroup(string str, object tp, string dir) {
     if(!str) {
          write("\n");
          i_MainMenu("2", tp);
          return 1;
     } else {
          write("\nEnter the access key: ");
          input_to("i_mdDirAKey", tp, dir, str);
          return 1;
     }

}

int i_mdDirAKey(string str, object tp, string dir, string group) {
     string *akeys = allocate(8);

     if(!str) {
          write("\n");
          i_MainMenu("2", tp);
          return 1;
     }

     //read, write, network, shadow, link, execute, bind, ownership
     if(strsrch(str, "r") != -1) akeys[0] = "r";
     if(strsrch(str, "w") != -1) akeys[1] = "w";
     if(strsrch(str, "n") != -1) akeys[2] = "n";
     if(strsrch(str, "s") != -1) akeys[3] = "s";
     if(strsrch(str, "l") != -1) akeys[4] = "l";
     if(strsrch(str, "e") != -1) akeys[5] = "e";
     if(strsrch(str, "b") != -1) akeys[6] = "b";
     if(strsrch(str, "o") != -1) akeys[7] = "o";

     if(!access[dir]) access += ([dir : ([group : akeys]) ]);
     else access[dir] += ([ group : akeys]);

     write("\nSuccess: Access to '" + dir +"' for group/user '" + group + "' updated.\n");
     i_MainMenu("2", tp);
     return 1;
}

int i_tgMembership(string str, object tp) {
     if(!str) {
          write("\n");
          i_MainMenu("2", tp);
          return 1;
     }

     if(!user_data_file(str)) {
          write("\nError: User '" + str + "' does not exist.\n");
          i_MainMenu("2", tp);
          return 1;
     } else {
          write("Enter the name of the group that you wish to toggle " + capitalize(str) + "'s membership to: ");
          input_to("i_tgMembership2", tp, str);
          return 1;
     }
}

int i_tgMembership2(string str, object tp, string user) {
     string *user_list;
     if(!str) {
          write("\n");
          i_MainMenu("2", tp);
          return 1;
     }

     user_list = groups[str];

     if(sizeof(user_list) == 0) {
          write("\nError:  Group '" + str + "' does not exist.\n");
          i_MainMenu("2", tp);
          return 1;
     }

     if(member_array(user, user_list) == -1) {
          groups[str] += ({user});
          write("\nSuccess: User '" + user + "' was added to group '" + str + "'\n");
          i_MainMenu("2", tp);
          return 1;
     } else {
          groups[str] -= ({user});
          write("\nSuccess: User '" + user + "' was removed from the group '" + str + "'\n");
          i_MainMenu("2", tp);
          return 1;
     }
}

int i_crGroup(string str, object tp) {
     if(!str) {
          write("\n");
          i_MainMenu("2", tp);
          return 1;
     }

     if(groups[str]) {
          write("\nError:  a group with the name '" + str + "' already exists.\n");
          i_MainMenu("2", tp);
          return 1;
     } else {
          groups[str] = ({query_privs(this_body())});
          write("\nSuccess:  group '" + str + "' created. Note: You were added to the new group.\n");
          i_MainMenu("2", tp);
          return 1;
     }
}

int i_dlGroup(string str, object tp) {
     if(!str) {
          write("\n");
          i_MainMenu("2", tp);
          return 1;
     }

     if(!groups[str]) {
          write("\nError:  group '" + str + "' does not exist.\n");
          i_MainMenu("2", tp);
          return 1;
     }

     if(str == "admin" || str == "soul") {
          write("\nError:  group '" + str + "' cannot be deleted.\n\t"
               + "The group is a system group and is required for proper functionality of the mudlib.\n");
          i_MainMenu("2", tp);
          return 1;
     } else {
          map_delete(groups, str);
          write("Success:  The group '" + str + "' was deleted.\n");
          i_MainMenu("2", tp);
          return 1;
     }
}

int i_cnDisMenu(string str, object tp) {
     i_MainMenu("1", tp);
}

int i_DisMenu(string str, object tp) {
     string *arr;
     arr = ({});

     switch(str) {
          case "1" :  {
               write("\nRaw group data:\n\n");
               write_gFile(1);
               write("\nRaw access data:\n\n");
               write_aFile(1);
               write("\n[Hit enter to continue]");
               input_to("i_cnDisMenu", tp);
               return 1;
          }

          case "2" :  {
               arr = keys(groups);
               if(!sizeof(arr)) arr = ({ "[None]" });
               write("\nThere is a total of " + sizeof(arr) + " groups on " + mud_name() + "\n\n");
               write(simple_list(arr));
               write("\n[Hit enter to continue]");
               input_to("i_cnDisMenu", tp);
               return 1;
          }

          case "3" : {
               write("\n Enter name of group: ");
               input_to("i_qGroup", tp);
               return 1;
          }
          case "4" : {
               write("\n Enter name of directory: ");
               input_to("i_qDirectory", tp);
               return 1;
          }
          default : {
               write("\n");
               main(tp, 0, 0);
               return 1;
          }
     }
}

int i_qDirectory(string str, object tp) {
     string *keys, *arr = ({});
     string output = "";
     mapping access_data;
     int j;

     if(!str) {
          write("\n");
          i_MainMenu("1", tp);
          return 1;
     }

     if(!mapp(access[str])) {
          write("\nError: No specific access settings have been set for directory '" + str +"'\n");
          write("\n[Hit enter to continue]");
          input_to("i_cnDisMenu", tp);
          return 1;
     }

     access_data = access[str];
     output += "(" + str + ") ";
     keys = keys(access_data);
     arr = ({});
     for(j = 0; j < sizeof(keys); j ++)
     arr += ({(sprintf("%s[%s]", keys[j], implode(access_data[keys[j]], "")))});
     if(sizeof(arr) > 1) output += sprintf("%s%s\n",implode(arr[0..(sizeof(arr)-2)], ":"), ":" + arr[sizeof(arr)-1]);
     else output += arr[0] + "\n";
     write("\n" + output);
     write("\n[Hit enter to continue]");
     input_to("i_cnDisMenu", tp);
     return 1;
}

int i_qGroup(string str, object tp) {
     string *arr;

     if(!str) {
          write("\n");
          i_MainMenu("1", tp);
          return 1;
     }

     if(!sizeof(groups[str])) {
          write("\nError: Group '" + str + "' doesn't exist.\n");
          write("\n[Hit enter to continue]");
          input_to("i_cnDisMenu", tp);
          return 1;
     }

     arr = groups[str];
     if(sizeof(arr) > 1) printf("\nThe following users are a member of the group '" + str + "':\n\t%s, and %s\n", implode(arr[0..(sizeof(arr)-2)], ", "), arr[sizeof(arr)-1]);
     else printf("\nThe following user is a member of the group '" + str + "':\n\t%s\n", arr[0]);
     write("\n[Hit enter to continue]");
     input_to("i_cnDisMenu", tp);
     return 1;

}

void write_gFile(int flag) {
     string file = "";
     string *group_list = keys(groups), *group_data;
     int i;

     i = 0;
     group_list = keys(groups);
     group_data = ({});
     file = "";


     for(i = 0; i < sizeof(group_list); i ++) {
          group_data = groups[group_list[i]];
          file += "(" + group_list[i] + ")";
          if(sizeof(group_data) > 1) file += sprintf("%s%s\n",implode(group_data[0..(sizeof(group_data)-2)], ":"), ":" + group_data[sizeof(group_data)-1]);
          else if(sizeof(group_data) == 1) file += group_data[0] + "\n";
          else error("ERROR: Group '" + group_list[i] + "' has no members!");
     }
     if(flag) write(file + "\n");
     else parse_files();
}

void write_aFile(int flag) {
     string file = "";
     string *access_list = keys(access), *keys, *arr = ({});
     mapping access_data;
     int i, j;

     i = 0;
     j = 0;
     access_data = ([]);
     access_list = keys(access);
     keys = ({});
     arr = ({});
     file = "";

     for(i = 0; i < sizeof(access_list); i++) {
          access_data = access[access_list[i]];
          file += "(" + access_list[i] + ") ";
          keys = keys(access_data);
          arr = ({});
          for(j = 0; j < sizeof(keys); j ++)
          arr += ({(sprintf("%s[%s]", keys[j], implode(access_data[keys[j]], "")))});
          if(sizeof(arr) > 1) file += sprintf("%s%s\n",implode(arr[0..(sizeof(arr)-2)], ":"), ":" + arr[sizeof(arr)-1]);
          else file += arr[0] + "\n";
     }
     if(flag) write(file + "\n");
     else parse_files();
}

string *parse(string str) {
     string *arr;
     int i;

     if(!str) {
          return ({});
     }

     arr = explode(str, "\n");

     for(i = 0; i < sizeof(arr); i++) {
          if(arr[i][0] == '#') {
               arr[i] = 0;
               continue;
          }
          arr[i] = replace_string(arr[i], " ", "");
          arr[i] = replace_string(arr[i], "\t", "");
     }
     return arr;
}

void parse_files() {
     parse_group();
     parse_access();
}

void parse_group() {
     int i, n;
     string *arr = parse(read_file(FILE_GROUPDATA));

#ifdef DEBUG
     write_file("/log/security", "\tDebug [security]: Parsing group data file...\n");
#endif

     groups = ([]);

     for(i = 0; i < sizeof(arr); i++) {
          string group, str, *members;

          if(!arr[i]) continue;

          if(sscanf(arr[i], "(%s)%s", group, str) != 2) {
               write("Error [security]: Invalid format of data in group data.\n");
               write("Security alert: Ignoring group on line " + (i + 1) + "\n");
               continue;
          }

          members = explode(str, ":");

#ifdef DEBUG
          write_file("/log/security", "Debug [security]: Adding group '" + group + "' with " + sizeof(members) + " members.\n");
#endif

          for(n = 0; n < sizeof(members); n++) {
               if(!file_size(user_data_file(members[n])) && !sscanf(members[n], "[%*s]")) {
                    write("Error [security]: Unknown user detected.\n");
                    write("Security alert: User '" + members[n] + "' ignored for group '" + group + "'.\n");
                    members -= ({ members[n] });
                    continue;
               }
#ifdef DEBUG
               write_file("/log/security", "Debug [security]: Adding user '" + members[n] + "' to group '" + group + "'.\n");
#endif
          }

          groups += ([group : members]);
     }
}

void parse_access() {
     int i, n;
     string *arr = parse(read_file(FILE_ACCESSDATA));

#ifdef DEBUG

     write_file("/log/security", "\tDebug [security]: Parsing access data file...\n");

#endif

     access = ([]);

     for(i = 0; i < sizeof(arr); i++) {
          string directory, str, *entries;
          mapping data;

          data = ([]);

          if(!arr[i]) continue;

          if(sscanf(arr[i], "(%s)%s", directory, str) != 2) {
               write("Error [security]: Invalid format of data in access data.\n");
               error("Security alert: Fatal error parsing access data on line " + (i + 1) + "\n");
          }

          if(str[<1..< 1] == ":") {
               write("Error [security]: Incomplete data in access data (trailing ':').\n");
               error("Security alert: Fatal error parsing access data on line " + (i + 1) + "\n");
          }

          entries = explode(str, ":");

#ifdef DEBUG

          write_file("/log/security", "Debug [security]: Parsing data for directory '" + directory + "'.\n");

#endif

          for(n = 0; n < sizeof(entries); n++) {
               string identity, permissions, *perm_array = allocate(8);
               if(sscanf(entries[n], "%s[%s]", identity, permissions) != 2) {
                    write("Error [security]: Invalid entry(" + n + ") data format in access data.\n");
                    error("Security alert: Fatal error parsing access data on line " + (i + 1) + "\n");
               }

#ifdef DEBUG

               write_file("/log/security", "Debug [security]: Adding identity '" + identity + "' with permission string of '" + permissions + "'.\n");

#endif

               //read, write, network, shadow, link, execute, bind, ownership
               if(strsrch(permissions, "r") != -1) perm_array[0] = "r";
               if(strsrch(permissions, "w") != -1) perm_array[1] = "w";
               if(strsrch(permissions, "n") != -1) perm_array[2] = "n";
               if(strsrch(permissions, "s") != -1) perm_array[3] = "s";
               if(strsrch(permissions, "l") != -1) perm_array[4] = "l";
               if(strsrch(permissions, "e") != -1) perm_array[5] = "e";
               if(strsrch(permissions, "b") != -1) perm_array[6] = "b";
               if(strsrch(permissions, "o") != -1) perm_array[7] = "o";

               data += ([ identity : perm_array ]);
          }

          access += ([directory : data]);
     }
}

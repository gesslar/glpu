/* ga_server.c

 Tacitus @ LPUniversity
 14-JAN-06
 Global Alias Server

*/

/* Preproccessor Statements */

#include <origin.h>

#define CONFIG_FILE "/adm/etc/aliases"

inherit STD_DAEMON;

/* Prototypes */

void parse_config();
string *parse(string str);
void add_alias(string verb, string cm, string *groups);
mapping get_alias(string priv);
mapping get_xverb(string priv);

/* Global Variables */

mapping xverb = ([]);
mapping alias = ([]);

/* Functions */

int setup() {
     set_no_clean(1);
     parse_config();
}

void parse_config() {
     int i, total_errors = 0, total_parsed = 0;
     string *conf;
     string *cur_groups = ({});
     string out = "";
     float time;

     time = time_frac();
     conf = parse(read_file(CONFIG_FILE));

     for(i = 0; i < sizeof(conf); i++) {
          string groups, verb, alias;

          if(!conf[i]) continue;

          if(sscanf(conf[i], ":;%s:", groups)) {
               cur_groups = explode(groups, " ");
               continue;
          }

          if(!sizeof(cur_groups)) {
               out += "\n";
               out += "\tGlobal Alias Server Error: No assignment definition found.\n";
               out += "\tGlobal Alias Server Error: Global Aliases were not parsed.\n";
               return;
          }

          if(sscanf(conf[i], "%s %s", verb, alias)) {
               add_alias(verb, alias, cur_groups);
               total_parsed ++;
          }

          else {
               out += "\n";
               out += "\tGlobal Alias Server Error: Definition found (" + i +") but in invalid format.\n";
               total_errors ++;
          }
     }

     out += "\nGlobal Alias Server: " + total_parsed + " global aliases parsed. " + total_errors + " errors encountered. " +
          sprintf("(%.2fms)\n", time_frac() - time);
}

string *parse(string str) {
     string *arr;
     int i;

     if(!str) {
          return ({});
     }

     arr = explode(str, "\n");

     for(i = 0; i < sizeof(arr); i++) {
          if(arr[i][0] == '#' || arr[i] == "") {
               arr[i] = 0;
               continue;
          }
          arr[i] = replace_string(arr[i], "\t", "");
     }
     return arr;
}


void add_alias(string verb, string cmd, string *groups) {
     int i;

     if(origin() != ORIGIN_LOCAL) return;

     if(verb[0] == '$' && strlen(verb) > 1) {
          if(!mapp(xverb)) xverb = ([]);

          for(i = 0; i < sizeof(groups); i++) {
               if(!xverb[groups[i]]) xverb += ([groups[i] : ([]) ]);
               xverb[groups[i]] += ([verb[1..<1] : cmd]);
          }
     }

     else {
          if(!mapp(alias)) alias = ([]);
          for(i = 0; i < sizeof(groups); i++) {
               if(!alias[groups[i]]) alias += ([groups[i] : ([]) ]);
               alias[groups[i]] += ([verb : cmd]);
          }
     }
}

mapping get_alias(string priv) {
     int i;
     string *keys = keys(alias);
     mapping ret = ([]);

     for(i = 0; i < sizeof(keys); i++)
          if(is_member(priv, keys[i]) || keys[i] == "all") ret += alias[keys[i]];

     return ret;
}

mapping get_xverb(string priv) {
     int i;
     string *keys = keys(xverb);
     mapping ret = ([]);

     for(i = 0; i < sizeof(keys); i++)
          if(is_member(priv, keys[i]) || keys[i] == "all") ret += xverb[keys[i]];

     return ret;
}

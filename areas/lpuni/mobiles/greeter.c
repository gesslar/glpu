/* greeter.c

 Tacitus @ LPuniversity
 28-JUNE-06
 Greeter Mob for LPuni Area

*/

inherit STD_NPC;

int count;
string *cmd_queue;
mapping messages;

void setup() {
    set_name("johnathon");
    set_short("Johnathon stands in front of the Podium");
    set_long(
      "Johnathon is positioned at the podium wearing his\n"
      "black tuxedo. His brown shaggy hair sways back and\n"
      "forth as he goes about seating guests.\n");
    set_id( ({"butler", "johnathon", "Johnathon", "john", "John" }) );

    if(clonep())
        set_heart_beat(1);

    count = 0;
    cmd_queue = ({});
    messages = ([]);
    add_hb("chatter") ;
}

void chatter() {
    if(count % (60 * 5) == 0) {
        command("say Welcome to the LPUniversity Cafe.");
        command("say Please feel free to make your way inside.");
    }

    if(sizeof(cmd_queue) > 0) {
        foreach(string cmd in explode(cmd_queue[0], "\n"))
            command(cmd);
        cmd_queue = cmd_queue[1..];
    }

    foreach(string key, string *arr in messages) {
        object user = find_player(key);

        if(sizeof(arr) <= 0) {
            map_delete(messages, key);
            continue;
        }

        if(!objectp(user) || !interactive(user)) continue;

        command("tell " + user->query_name() + " " + arr[0]);
        messages[key] = messages[key][1..];
    }

    count++;
}

void event_message(object prev, string msg, int msg_class, object tp) {
    string target, arg, from, chan;

    if(tp == this_object())
        return;

    msg = COLOUR_D->substitute_colour(msg, "plain") ;

    if(sscanf(msg, "%s bows%*sto you", target))
        cmd_queue += ({ "bow " + lower_case(target) });

    if(sscanf(msg, "%*s slaps you%*s"))
        cmd_queue += ({ "frown\nsay Please do not slap me." });


    if(sscanf(msg, "%s has entered%*s", target) || sscanf(msg, "%s has entered %*s", target))
        cmd_queue += ({ "smile\nsay Welcome to the LPUniversity Cafe." });

    if(sscanf(msg, "%s has gone link-dead.", target)) {
        cmd_queue += ({ "emote sweeps " + target + " away." });
        find_player(lower_case(target))->move(VOID_ROOM);
    }

    if(sscanf(msg, "%s pats you on the head.", target))
        if(adminp(lower_case(target))) cmd_queue += ({ "smile" });

    if(sscanf(msg, "%s says: tell %s \"%s\"", from, target, arg) == 3 ||
        sscanf(msg, "%s says: Tell %s \"%s\"", from, target, arg) == 3) {
        target = lower_case(target);
        from = lower_case(from);

        if(from == target) {
            cmd_queue += ({"chuckle\nsay " + arg});
            return;
        }

        if(!messages[target]) messages += ([ target : ({}) ]);
        messages[target] += ({ capitalize(from) + " asked me to tell you: '" + arg + "'." });
        cmd_queue += ({ "nod\nsay The next time I see " + capitalize(target)
            + " I'll tell them that." });
    }

    if(sscanf(msg, "[%s] %s: tell %s \"%s\"", chan, from, target, arg) == 4 ||
        sscanf(msg, "[%s] %s: Tell %s \"%s\"", chan, from, target, arg) == 4) {
        target = lower_case(target);

        if(!messages[target]) messages += ([ target : ({}) ]);
        messages[target] += ({ capitalize(from) + " asked me to tell you: '" + arg + "' at " + ctime(time()) + "." });
        cmd_queue += ({ lower_case(chan) + " The next time I see " + capitalize(target)
            + ", on this mud, I'll tell them that." });
    }
}

/* Do not remove the headers from this file! see /USAGE for more info. */

inherit STD_DAEMON;

#include <daemons.h>

/* General message handling.  Inherit it in anything that needs it.
 *
 * -Beek
 */

//:MODULE
// The message module.  The correct way to compose and send any messages
// To users is using this module, as it will automatically get the grammar
// right for each person involved.

private string SHORT(mixed x) {
    if(objectp(x)) {
        if(living(x)) return x->query_name();
        else return x->query_name() || x->query_short();
    }
    return x;
}

string _a_short(mixed x) {
    if(objectp(x)) return x->query_short();
// Check for "a"/"an" prefix - if missing, add it
    if(stringp(x)) return add_article(remove_article(x));
    return x;
}

string _the_short(mixed x) {
    if(objectp(x)) return x->query_short();
    // Check for "the" prefix - if missing, add it
    if(stringp(x)) return add_article(remove_article(x), 1);
    return x;
}

mixed *handle_obs(mixed *obs, string res, mapping has) {
    string *ret = ({});
    mapping _items = ([]);
    string t_short;

    if(objectp(obs[0]) && has[obs]) {
        ret = ({"them"});
    } else {
        foreach(mixed ob in obs) {
            t_short = SHORT(ob);
            if(member_array(t_short, keys(_items)) < 0) _items += ([ t_short : 1 ]);
            else _items[t_short] ++;
        }
        obs = keys(_items);

        if(res[<2..<1]=="a ") {
            res = res[0..<3];
            foreach(mixed ob in obs) {
                if(_items[ob]>1) ret += ({ _items[ob] + " " + pluralize(SHORT(ob)) });
                else ret +=  ({ _a_short(ob) });
            }
        } else if(res[<4..<1] == "the ") {
            res = res[0..<5];
            foreach(mixed ob in obs) {
                if(_items[ob]>1) ret += ({ "the " + _items[ob] + " " + pluralize(SHORT(ob)) });
                else ret += ({ _the_short(ob) });
            }
        } else if(res[<2..<1] == "A ") {
            res = res[0..<3];
            foreach(mixed ob in obs) {
                if(_items[ob]>1) ret += ({ _items[ob] + " " + pluralize(SHORT(ob)) });
                else ret += ({ capitalize(_a_short(ob)) });
            }
        } else if(res[<4..<1] == "The ") {
            res = res[0..<5];
            foreach(mixed ob in obs) {
                if(_items[ob]>1) ret += ({ "the " + _items[ob] + " " + pluralize(SHORT(ob)) });
                else ret += ({ capitalize(_the_short(ob)) });
            }
        } else {
            foreach(mixed ob in obs) {
                if(_items[ob]>1) ret += ({ _items[ob] + " " + pluralize(SHORT(ob)) });
                else ret += ({ SHORT(ob) });
            }
        }
        has[obs]++;
    }

    return ({ res, ret });
}

mixed *handle_ob(mixed ob, string res, mapping has) {
    string bit;

    if(objectp(ob) && has[ob]) {
        bit = "it";
    } else {
        if(res[<2..<1]=="a ") {
            res = res[0..<3];
            bit = _a_short(ob);
        } else if(res[<4..<1] == "the ") {
            res = res[0..<5];
            bit = _the_short(ob);
        } else if(res[<2..<1] == "A ") {
            res = res[0..<3];
            bit = capitalize(_a_short(ob));
        } else if(res[<4..<1] == "The ") {
            res = res[0..<5];
            bit = capitalize(_the_short(ob));
        } else {
            bit = SHORT(ob);
        }
        has[ob]++;
        has[bit]++;
    }

    return ({ res, bit });
}

//:FUNCTION compose_message
//The lowest level message composing function; it is passed the object
//for whom the message is wanted, the message string, the array of people
//involved, and the objects involved.  It returns the appropriate message.
//Usually this routine is used through the higher level interfaces.
varargs string compose_message(object forwhom, string msg, object *who, mixed *obs...) {
    mixed ob;
    mixed *fmt;
    string res;
    int i;
    int c;
    int num, subj;
    string str;
    string bit;
    mapping has = ([]);
    mixed tmp;

    fmt = reg_assoc(msg, ({ "\\$[NnVvTtPpOoRrBb][a-z0-9]*" }), ({ 1 }));
    fmt = fmt[0] ; // ignore the token info for now

    res = fmt[0];
    i = 1;
    while(i < sizeof(fmt)) {
        c = fmt[i][1];
        if(fmt[i][2] && fmt[i][2] < 'a') {
            if(fmt[i][3] && fmt[i][3] < 'a') {
                subj = fmt[i][2] - '0';
                num = fmt[i][3] - '0';
                str = fmt[i][4..<0];
            } else {
                subj = 0;
                num = fmt[i][2] - '0';
                str = fmt[i][3..<0];
            }
        } else {
            subj = 0;
            num = ((c == 't' || c == 'T') ? 1 : 0) ; // target defaults to 1, not zero
            str = fmt[i][2..<0];
        }
        switch (c) {
            case 'o':
            case 'O':
                ob = obs[num];
                if(pointerp(ob)) {
                    if(sizeof(ob)) {
                        tmp = handle_obs(ob, res, has);
                        res = tmp[0];
                        ob = tmp[1];
                        bit = simple_list(ob);
                    } else {
                        tmp = ({ res });
                        for(int z = 0; z < sizeof(ob); z++) {
                            tmp = handle_ob(ob[z], res, has);
                            ob[z] = tmp[1];
                        }
                        res = tmp[0];
                        bit = simple_list(ob);
                    }
                } else {
                    tmp = handle_ob(ob, res, has);
                    res = tmp[0];
                    bit = tmp[1];
                }
                break;
            case 't':
            case 'T':
            /* Only difference between $n and $t is that $t defaults to $n1o */
            /* Fall through */
                if(str=="") str = "o";
            case 'n':
            case 'N':
                if(str=="") str = "s";
                if(str != "p") {
                    if(str!="d") {
                        /* Handle reflexification */
                        if(subj < sizeof(who) && (who[subj] == who[num]) && has[who[subj]]) {
                            // objective: You kick yourself, Beek kicks himself.
                            if(str == "o") {
                                if(forwhom == who[subj]) bit = "yourself";
                                else bit = reflexive(who[subj]);
                            }
                            // subjective: You prove you are stupid,
                            // Beek proves he is stupid.
                            if(str == "s") {
                                if(forwhom == who[subj]) bit = "you";
                                else bit = subjective(who[subj]);
                            }
                            break;
                        }
                        /* Other pronouns */
                        if(who[num]==forwhom) {
                            bit = "you";
                            has[who[num]]++;
                            break;
                        }
                        if(has[who[num]]) {
                            if(str[0]=='o') bit = objective(who[num]);
                            else bit = subjective(who[num]);
                            break;
                        }
                        has[who[num]]++;
                        bit = who[num]->query_name();
                        break;
                    }
                }
                has[who[num]]++;
                bit = who[num]->query_name();
                break;
            case 'R':
            case 'r':
                if(forwhom == who[num]) bit = "yourself";
                else bit = reflexive(who[num]);
                break;
            case 'v':
            case 'V':
                /* hack for contractions */
                if(i + 1 < sizeof(fmt) && fmt[i+1][0..2] == "'t ") {
                    str += "'t";
                    fmt[i+1] = fmt[i+1][2..];
                }
                /* hack for to be */
                if(str == "is" || str == "am" || str == "are") {
                    if(num >= sizeof(who) || who[num]==forwhom || who[num]->query_gender() == "other") bit = "are";
                    else bit = "is";
                } else {
                    if(num >= sizeof(who) || who[num]!=forwhom || who[num]->query_gender() == "other") bit = pluralize(str);
                    else bit = str;
                }
                break;
            case 'p':
            case 'P':
                //Bit of defensive coding here
                if(num>=sizeof(who)) {
                    bit = "WHOSE(?)";
                    break;
                }
                else {
                    if(forwhom == who[num]) {
                        bit = "your";
                        break;
                    }
                    if(has[who[num]]) {
                        bit = possessive(who[num]);
                        break;
                    }
                    bit = possessive_proper_noun(who[num]);
                    has[who[num]]++;
                    break;
                }
                break;
            case 'b':
            case 'B':
                //Bit of defensive coding here
                if(num>=sizeof(who)) {
                    bit = "WHOSE(?)";
                    break;
                } else {
                    if(forwhom == who[num]) {
                        bit = "you";
                        break;
                    }
                    if(has[who[num]]) {
                        bit = objective(who[num]);
                        break;
                    }
                    bit = who[num]->query_name();
                    has[who[num]]++;
                    break;
                }
                break;
        }
        // hack to prevent errors.
        if(!bit) bit = "";
        if(c < 'a') bit = capitalize(bit);
        res += bit + fmt[i+1];
        i += 2;
    }
    return append(res, "\n");
}

//### This now always indents continuation lines.  Might want a flag at the
//### end to enable or disable that.
//:FUNCTION inform
//Given an array of participants, and an array of messages, and either an
//object or array of objects, deliver each message to the appropriate
//participant, being careful not to deliver a message twice.
//The last arg is either a room, in which that room is told the 'other'
//message, or an array of people to recieve the 'other' message.
void inform(object *who, string *msgs, mixed others) {
    int i;
    mapping done = ([]);

    i = sizeof(who);
    while(i--) {
        if(done[who[i]]) continue;
        done[who[i]]++;
        tell(who[i], msgs[i]);
    }
    if(pointerp(others)) map_array(others - who, (: tell_down($1, $2, null, $3) :), msgs[<1], who);
    else if(others) tell_all(others, msgs[sizeof(who)], null, who);
}

//:FUNCTION action
//Make the messages for a given group of people involved.  The return
//value will have one array per person, as well as one for anyone else.
//inform() can be used to send these messages to the right people.
//see: inform

varargs string *action(object *who, mixed msg, mixed *obs...) {
    int i;
    string *res;

    if(pointerp(msg)) msg = element_of(msg);
    res = allocate(sizeof(who) + 1);
    for(i = 0; i < sizeof(who); i++) res[i] = compose_message(who[i], msg, who, obs...);
    res[sizeof(who)] = compose_message(0, msg, who, obs...);
    return res;
}

//:FUNCTION simple_action
//Generate and send s for an action involving the user and possibly
//some objects
varargs void simple_action(mixed msg, mixed *obs...) {
    string us;
    string others;
    object *who;

    if(!sizeof(msg)) return;
    /* faster to only make who once */
    who = ({ previous_object() });
    if(pointerp(msg)) msg = msg[random(sizeof(msg))];

    us = compose_message(previous_object(), msg, who, obs...);
    others = compose_message(0, msg, who, obs...);

    tell(previous_object(), us);
    tell_down(all_inventory(previous_object()), others);
    if(environment(previous_object())) {
        tell_all(environment(previous_object()), others, null, who);
    }
}

//:FUNCTION my_action
//Generate and send a message that should only be seen by the person doing it
varargs void my_action(mixed msg, mixed *obs...) {
    string us;
    object *who;

    if(!sizeof(msg)) return;
    who = ({ previous_object() });
    if(pointerp(msg)) msg = msg[random(sizeof(msg))];
    us = compose_message(previous_object(), msg, who, obs...);
    tell(previous_object(), us);
}

//:FUNCTION target_action
//Generate and send a message that should only be seen by the target
varargs void target_action(mixed msg, object target, mixed *obs...) {
    string them;
    object *who;

    if(!sizeof(msg)) return;
    who = ({ previous_object(), target });
    if(pointerp(msg)) msg = msg[random(sizeof(msg))];
    them = compose_message(target, msg, who, obs...);

    target->tell(them);
}

//:FUNCTION my_target_action
//Generate and send a target-based message that should only be seen by the sender
varargs void my_target_action(mixed msg, object target, mixed *obs...) {
    string us;
    object *who;

    if(!sizeof(msg)) return;
    who = ({ previous_object(), target });
    if(pointerp(msg)) msg = msg[random(sizeof(msg))];
    us = compose_message(previous_object(), msg, who, obs...);

    tell(previous_object(), us);
}

//:FUNCTION other_action
//Generate and send a message that should only be seen by others
varargs void other_action(mixed msg, mixed *obs...) {
    string others;
    object *who;

    if(!sizeof(msg)) return;
    who = ({ previous_object() });
    if(pointerp(msg)) msg = msg[random(sizeof(msg))];
    others = compose_message(0, msg, who, obs...);
    tell_down(all_inventory(previous_object()), others);
    if(environment(previous_object())) {
        tell_all(environment(previous_object()), others, null, who);
    }
}

//:FUNCTION targetted_action
//Generate and send a message involving the doer and a target (and possibly
//other objects)
varargs void targetted_action(mixed msg, object target, mixed *obs...) {
    string us, them, others;
    object *who;

    if(!sizeof(msg)) return;
    who = ({ previous_object(), target });
    if(pointerp(msg)) msg = msg[random(sizeof(msg))];
    us = compose_message(previous_object(), msg, who, obs...);
    them = compose_message(target, msg, who, obs...);
    others = compose_message(0, msg, who, obs...);

    tell(previous_object(), us);
    tell(target, them);
    tell_down(all_inventory(previous_object()), others);
    tell_down(all_inventory(target), others);
    if(environment(previous_object())) {
        tell_all(environment(previous_object()), others, null, who);
    }
}

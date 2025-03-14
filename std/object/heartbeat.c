// /std/object/heartbeat.c
// Heartbeat events
//
// Structure:
// hb_events = ({ ({ current, interval, function }), ... })
//
// Created:     2024/03/03: Gesslar
// Last Change: 2024/03/03: Gesslar
//
// 2024/03/03: Gesslar - Created

private nomask nosave mixed *hb_events = ({});

public varargs void add_hb(mixed f, int interval);
void remove_hb(mixed f);

// Add a heartbeat event
public varargs void add_hb(mixed f, int interval) {
    if(!valid_function(f) && !stringp(f))
        return;

    remove_hb(f);

    if(nullp(interval))
        interval = 1;

    hb_events += ({ ({ 0, interval, f }) });
}

void remove_hb(mixed f) {
    if(!valid_function(f) && !stringp(f))
        return;

    hb_events = filter(hb_events, (: $1[1] != $2 :), f);
}

// Heartbeat event
protected evaluate_heart_beat() {
    int i, sz;
    if(userp())
        return;

    for(i = 0, sz = sizeof(hb_events); i < sz; i++) {
        hb_events[i][0]++;
        if(hb_events[i][0] >= hb_events[i][1]) {
            if(stringp(hb_events[i][2]))
                catch(call_other(this_object(), hb_events[i][2]));
            else {
                function f;

                if(!valid_function(hb_events[i][2]))
                    continue;

                catch {
                    f = bind(hb_events[i][2], this_object());
                    (*f)();
                };
            }

            hb_events[i][0] = 0;
        }
    }
}

mixed *query_hb() {
    return copy(hb_events);
}

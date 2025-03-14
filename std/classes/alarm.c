#ifndef __ALARM_C__
#define __ALARM_C__

class Alarm {
    string type;
    string file;
    string func;
    mixed *args;
    int master;
    int last_run;
    string pattern;
    string id;
}

#endif // __ALARM_C__

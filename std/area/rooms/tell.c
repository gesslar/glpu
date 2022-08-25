// /std/area/rooms/tell.c
// Room tells
//
// Created:     2022/08/24: Gesslar
// Last Change: 2022/08/24: Gesslar
//
// 2022/08/24: Gesslar - Created

varargs int tell(string message, mixed args ...) {
    object *excludes = ({ }), *obs ;
    string message_class;
    int num_args = (!undefinedp(args) && pointerp(args)) ? sizeof(args) : 0;

    if(num_args) 
    {
        if(stringp(args[0]))
        {
            message_class = args[0];
        }
        else if(objectp(args[0]))
        {
            excludes = ({ args[0] });
        }
        else if (pointerp(args[0])) 
        {
            args[0] -= ({ 0 });
            args[0] = filter_array(args[0], "filter_object_args", this_object());
            if(sizeof(args[0]))
            {
                excludes = args[0];
            }
        }
        if(num_args >= 2)
        {
            if(stringp(args[1]))
            {
                message_class = args[1];
            }
        }
    }

    obs = all_inventory();
    
    if(message_class)
    {
        message(message_class, message, obs, excludes);
    }
    else
    {
        message("tell_room", message, obs, excludes);
    }
}

protected int filter_object_args(mixed arg) { return objectp(arg); }

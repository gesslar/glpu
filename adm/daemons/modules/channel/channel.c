/**
 * @file /adm/daemons/modules/channel/channel.c
 * @description Base inheritable for channel modules.
 *
 * @created 2024-09-10 - Gesslar
 * @last_modified 2024-09-10 - Gesslar
 *
 * @history
 * 2024-09-10 - Gesslar - Created
 */

inherit STD_DAEMON;

protected nosave string module_name = query_file_name();
protected nosave string *channel_names = ({});
private mapping history = ([]);

void mudlib_setup() {
  if(append(file_name(), ".c") == __FILE__)
    return;

  set_persistent();
  set_no_clean(1);
}

void post_setup_1() {
  if(append(file_name(), ".c") == __FILE__)
    return;

  CHAN_D->register_module(module_name, file_name());

  foreach(string channel in channel_names) {
    CHAN_D->register_channel(module_name, channel);

    if(nullp(history[channel]))
      history[channel] = ({});
  }
}

void add_history(string channel, string message) {
  if(nullp(history[channel]))
    history[channel] = ({});

  history[channel] += ({ message });
}

string *last_messages(string channel, int num_lines) {
  num_lines = clamp(10, 100, num_lines);

  if(!sizeof(history[channel]))
    return ({ "Channel " + channel + " has no history yet." });

  return history[channel][<num_lines..];
}

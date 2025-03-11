/**
 * @file /lpu/lib/cmds/std/history.c
 *
 * Command implementation for viewing a player's command history.
 * Allows viewing full history or specific ranges of past commands.
 *
 * @created 2006-08-12 - Tacitus
 * @last_modified 2024-03-11
 *
 * @history
 * 2006-08-12 - Tacitus - Created
 */

inherit STD_CMD;

/**
 * Displays command history for the calling player.
 *
 * Shows a numbered list of previously executed commands. Can display:
 * - Last 15 commands (default)
 * - Full history (with "all" argument)
 * - Custom range (with numeric argument)
 *
 * @param {object} caller - The player viewing their history
 * @param {string} args - Optional argument: "all" or number of entries to show
 * @returns {int|string} 1 on success, error message on failure
 * @example
 * history         // Shows last 15 commands
 * history all     // Shows entire history
 * history 25      // Shows last 25 commands
 */
mixed main(object caller, string args) {
  int range, i;
  string *history = ({});

  history = this_body()->query_command_history();

  if(stringp(args)) {
    if(args == "all") {
      for(i = 0; i < sizeof(history); i++)
        printf(" %-5d %-5s %s\n", i, ":",  history[i]);
    } else {
      range = to_int(args);
      if(!intp(range) || range < 0)
        return _error("Invalid argument type.");
      else {
        for(i = sizeof(history) - (range + 1); i < sizeof(history); i++)
          printf(" %-5d %-5s %s\n", i, ":",  history[i]);
      }
    }
  } else {
    for(sizeof(history) > 16 ? i = sizeof(history) - 16 : i = 0; i < sizeof(history); i++)
      printf(" %-5d %-5s %s\n", i, ":",  history[i]);
  }

  return 1;
}

/**
 * Provides help documentation for the history command.
 *
 * @param {object} caller - The player requesting help
 * @returns {string} Help text explaining command usage
 */
string help(object caller) {
    return(" SYNTAX: history [range]\n\n"
    "This command allows you to view the history of commands that\n"
    "you've executed. By default is displays the last 15 commands\n"
    "executed by you but you may provide a custom ammount by providing\n"
    "an argument of 'all' to view the entire history or a number of your\n"
    "choice.\n");
}

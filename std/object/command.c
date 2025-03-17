/**
 * @file /std/object/command.c
 * @description Replacement for add_action, allowing for mixed results and more
 *              flexible command handling.
 *
 * @created 2024-03-03 - Gesslar
 * @last_modified 2025-03-16 - GitHub Copilot
 *
 * @history
 * 2024-03-03 - Gesslar - Created
 * 2025-03-16 - GitHub Copilot - Added documentation
 */

#include <command.h>

private nosave mapping _commands = ([]);

/**
 * Adds a command handler to this object.
 *
 * @param {string|string*} command - Command name or array of command names
 * @param {function|string} action - Function pointer or method name to handle command
 * @errors If action function does not exist
 * @errors If command or action parameters are invalid types
 */
void add_command(mixed command, mixed action) {
  if(stringp(command)) {
    remove_command(command);
    if(stringp(action)) {
      if(!function_exists(action)) {
        error("add_command: No such function " + action + " in " + file_name() + ".\n");
      }
      _commands[command] = action;
    } else if(valid_function(action)) {
      _commands[command] = action;
    } else {
      error("add_command: Illegal action " + action + " in " + file_name() + ".\n");
    }
  } else if(pointerp(command)) {
    foreach(mixed cmd in command) {
      add_command(cmd, action);
    }
  } else {
    error("add_command: Illegal command " + command + " in " + file_name() + ".\n");
  }
}

/**
 * Removes one or more commands from this object.
 *
 * @param {string|string*} command - Command name or array of command names
 */
void remove_command(mixed command) {
  mixed action;

  if(stringp(command)) {
    action = _commands[command];
    map_delete(_commands, command);
  } else if(pointerp(command)) {
    foreach(mixed cmd in command) {
      if(stringp(cmd)) {
        action = _commands[cmd];
        map_delete(_commands, cmd);
      }
    }
  }
}

/**
 * Removes all commands that use a specific action handler.
 *
 * @param {function|string} action - Function name or pointer to remove
 */
void remove_command_all(mixed action) {
  foreach(mixed command, mixed act in _commands) {
    if(act == action) {
      map_delete(_commands, command);
      remove_command(command);
    }
  }
}

/**
 * Returns the action associated with a command.
 *
 * @param {string} command - The command to query
 * @returns {function|string|null} Action handler or null if not found
 */
mixed query_command(string command) {
  return _commands[command];
}

/**
 * Returns all registered commands and their actions.
 *
 * @returns {mapping} Copy of commands mapping
 */
mapping query_commands() {
  return copy(_commands);
}

/**
 * Finds all commands that share the same action handler.
 *
 * Returns an array containing the given command and any other commands
 * that use the same action handler.
 *
 * @param {string} command - The command to find matches for
 * @returns {string*} Array of commands sharing the same handler, or empty array
 */
string *query_matching_commands(string command) {
  string *matches = allocate(1);

  if(nullp(_commands[command]))
    return ({});

  matches[0] = command;

  foreach(mixed cmd, mixed action in _commands) {
    if(cmd == matches[0])
      continue;

    if(action == _commands[command])
      matches += ({ cmd });
  }

  return matches;
}

/**
 * Initialises the commands mapping to an empty state.
 */
void init_commands() {
  _commands = ([]);
}

/**
 * Evaluates a command by calling its associated action.
 *
 * @param {STD_PLAYER} user - The object triggering the command
 * @param {string} command - The command name
 * @param {string} arg - The command arguments
 * @returns {mixed} Result of command evaluation, or null if no handler
 */
mixed evaluate_command(object user, string command, string arg) {
  mixed action = _commands[command];

  if(stringp(action))
    return call_other(this_object(), action, user, arg);
  else if(valid_function(action))
    return (*action)(user, arg);

  return null;
}

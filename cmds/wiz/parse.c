/**
 * @file /cmds/wiz/parse.c
 *
 * Tests a parse to see what it results in.
 *
 * @created 2025-03-18 - Gesslar
 * @last_modified 2025-03-18 - Gesslar
 *
 * @history
 * 2025-03-18 - Gesslar - Created
 */

mixed main(object tp, string str) {
  if(!str)
    return "Parse what?";

  return this_body()->verb_hook(str, 3) || "No parsed result.";
}

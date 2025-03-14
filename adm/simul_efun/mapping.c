#include <simul_efun.h>

private string regex = "( \\/\\* sizeof\\(\\) == \\d+ \\*/)";

/**
 * Returns a formatted string representation of a mapping, removing any size
 * annotations.
 *
 * @param {mapping} map - The mapping to format.
 * @returns {string} The formatted string representation of the mapping.
 */
string pretty_map(mapping map) {
  string str = sprintf("%O\n", map);

  while(pcre_match(str, regex))
    str = pcre_replace(str, regex, ({""}));

  return str;
}

/**
 * @file /adm/daemons/lines.c
 * @description Daemon to handle line-drawing.
 *
 * I know I stole this from someone, but I don't know who. Maybe Lima? idk.
 * Could have been @michaelprograms, but I'm not sure. Shout out tho whomever
 * though! This is a great module.
 *
 * It is somewhat reminiscent of LP Terminal mappings, inasmuch as it's kind
 * of exactly the same thing.
 *
 * @created 2024-07-13 - Gesslar
 * @last_modified 2024-07-13 - Gesslar
 *
 * @history
 * 2024-07-13 - Gesslar - Created
 */

inherit STD_DAEMON;

private nosave mapping line_drawing = ([ ]);

void setup() {
    line_drawing["UTF-8"] = ([
        // lines
        "─" : "─",
        "═" : "═",
        "│" : "│",
        "║" : "║",
        // diagonals
        "╲" : "╲",
        "╱" : "╱",
        // corners
        "┌" : "┌",
        "╔" : "╔",
        "╓" : "╓",
        "╒" : "╒",
        "┐" : "┐",
        "╗" : "╗",
        "╕" : "╕",
        "╖" : "╖",
        "┘" : "┘",
        "╝" : "╝",
        "╛" : "╛",
        "╜" : "╜",
        "└" : "└",
        "╚" : "╚",
        "╘" : "╘",
        "╙" : "╙",
        // joints
        "┬" : "┬",
        "╤" : "╤",
        "╥" : "╥",
        "╦" : "╦",
        "┴" : "┴",
        "╧" : "╧",
        "╨" : "╨",
        "╩" : "╩",
        "├" : "├",
        "╞" : "╞",
        "╟" : "╟",
        "╠" : "╠",
        "┤" : "┤",
        "╡" : "╡",
        "╢" : "╢",
        "╣" : "╣",
        "┼" : "┼",
        "╬" : "╬",
        "╪" : "╪",
        "╫" : "╫",
        "╳" : "╳",
    ]);

    line_drawing["US-ASCII"] = ([
        // lines
        "─" : "-",
        "═" : "=",
        "│" : "|",
        "║" : "|",
        // diagonals
        "╲" : "\\",
        "╱" : "/",
        // corners
        "┌" : ".",
        "╔" : ".",
        "╓" : ".",
        "╒" : ".",
        "┐" : ".",
        "╗" : ".",
        "╕" : ".",
        "╖" : ".",
        "┘" : "'",
        "╝" : "'",
        "╛" : "'",
        "╜" : "'",
        "└" : "'",
        "╚" : "'",
        "╘" : "'",
        "╙" : "'",
        "╚" : "'",
        // joints
        "┬" : "-",
        "╤" : "-",
        "╥" : "-",
        "╦" : "-",
        "┴" : "-",
        "╧" : "-",
        "╨" : "-",
        "╩" : "-",
        "├" : "|",
        "╞" : "|",
        "╟" : "|",
        "╠" : "|",
        "┤" : "|",
        "╡" : "|",
        "╢" : "|",
        "╣" : "|",
        "┼" : "+",
        "╬" : "+",
        "╪" : "+",
        "╫" : "+",
        "╳" : "╳",
    ]);

    line_drawing["screenreader"] = allocate_mapping(keys(line_drawing["UTF-8"]), " ");
}

private string replace_lines_characters(string mess, mapping replacement) {
    string k, v;

    foreach(k, v in replacement) {
        while(strsrch(mess, k) > -1) mess = replace_string(mess, k, v);
    }

    return mess;
}

string substitute_lines(string mess, string encoding) {
    if(encoding == "UTF-8") return mess;
    if(of(encoding, line_drawing)) return replace_lines_characters(mess, line_drawing[encoding]);
    return replace_lines_characters(mess, line_drawing["US-ASCII"]);
}

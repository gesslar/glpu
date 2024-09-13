/**
 * @file /adm/daemons/colour.c
 * @description Colour Parser for handling and converting colour codes in text.
 *
 * @created 2022-08-22 - Gesslar
 * @last_modified 2022-08-22 - Gesslar
 *
 * @history
 * 2022-08-22 - Gesslar - Created
 */

#include <colour.h>

inherit STD_DAEMON ;
inherit DM_CSS ;

// Forward declarations
string substitute_colour(string text, string mode);
string wrap(string str, int wrap_at, int indent_at);
void resync();
int colourp(string text);
string token_to_colour(string token);
string get_colour_list();
int is_too_dark(string colour);
string substitute_too_dark(string text);
int colour_to_greyscale(int colour_code);
string body_colour_replace(object body, string text, int message_type);
private int too_dark_check();
private mapping too_dark_map();
public int *colour_to_rgb(int color_code);
public int rgb_to_colour(int r, int g, int b);

private nosave string *fg_codes = ({ }) ;
private nosave string *bg_codes = ({ }) ;
private nosave mapping alt_codes = ([ ]) ;
private nosave string *low_codes = ({ }) ;
private nosave mapping high_to_low_fg = ([ ]) ;
private nosave mapping high_to_low_bg = ([ ]) ;

private void load_all_colours() ;

void setup() {
    set_no_clean() ;
    load_all_colours() ;
}

private int too_dark_check() {
    return mud_config("COLOUR_TOO_DARK") == "on" ;
}

private mapping too_dark_map() {
    return mud_config("COLOUR_TOO_DARK_SUB") ;
}

private void load_all_colours() {
    int i ;
    string *lines ;

    // First let's load all the 256 colours into the colour codes
    i = 256 ;
    fg_codes = allocate(i) ;
    bg_codes = allocate(i) ;
    while(i--) {
        fg_codes[i] = sprintf("\e[38;5;%dm", i) ;
        bg_codes[i] = sprintf("\e[48;5;%dm", i) ;
    }

    alt_codes = ([
        "res" : "\e[0m",  // reset
        "re0" : "\e[27m", // reverse video off
        "re1" : "\e[7m",  // reverse video on
        "it0" : "\e[23m", // italics off
        "it1" : "\e[3m",  // italics on
        "ul0" : "\e[24m", // underline off
        "ul1" : "\e[4m",  // underline on
        "fl0" : "\e[25m", // flash off
        "fl1" : "\e[5m",  // flash on
        "st0" : "\e[29m", // strike off
        "st1" : "\e[9m",  // strike on
        "bl0" : "\e[22m", // bold off
        "bl1" : "\e[1m",  // bold on
        "di0" : "\e[22m", // dim off
        "di1" : "\e[2m",  // dim on
        "ol0" : "\e[55m", // overline off
        "ol1" : "\e[53m", // overline on
    ]) ;

    lines = explode(read_file("/adm/etc/colour/high_to_low_map.txt"), "\n") ;
    i = 256 ;
    low_codes = allocate(i) ;
    while(i--) {
        string low ;

        sscanf(lines[i], "%*s,%s", low) ;

        low_codes[i] = low ;
    }

    lines = explode(read_file("/adm/etc/colour/high_to_low_fg.txt"), "\n") ;
    i = 16 ;
    while(i--) {
        string high ;
        string low ;

        sscanf(lines[i], "%s,%s", high, low) ;
        high_to_low_fg[high] = sprintf("\e[%sm", low) ;
    }

    lines = explode(read_file("/adm/etc/colour/high_to_low_bg.txt"), "\n") ;
    i = 16 ;
    while(i--) {
        string high ;
        string low ;

        sscanf(lines[i], "%s,%s", high, low) ;
        high_to_low_bg[high] = sprintf("\e[%sm", low) ;
    }
}

/**
 * @daemon_function substitute_colour
 * @description Substitutes colour tokens in text with appropriate colour codes.
 * @param {string} text - The text containing colour tokens.
 * @param {string} mode - The mode for colour substitution (plain, vt100, high, low).
 * @returns {string} The text with colour tokens replaced by appropriate codes.
 */
public string substitute_colour(string text, string mode) {
    mixed *assoc ;
    string *parts, sub ;
    int *matched ;
    int sz, num ;
    string result ;

    if(nullp(text)) return "" ;
    if(nullp(mode)) mode = "plain" ;

    assoc = pcre_assoc(text, ({ COLOUR_REGEX }), ({ 1 })) ;
    parts = assoc[0] ;
    matched = assoc[1] ;
    sz = sizeof(parts) ;

    switch(mode) {
        case "plain" :
            while(sz--) {
                // Skip non matches
                if(matched[sz] == 0) continue ;

                parts[sz] = "" ;
            }
            break ;
        case "vt100" :
            while(sz--) {
                // Skip non matches
                if(matched[sz] == 0) continue ;

                // First, are we an alt code
                if(sscanf(parts[sz], "{{%s}}", sub) == 1) {
                    if(!nullp(sub = alt_codes[sub])) {
                        parts[sz] = sub ;
                        continue ;
                    }
                }

                parts[sz] = "" ;
            }
            break ;
        case "high" :
            while(sz--) {
                // Skip non matches
                if(matched[sz] == 0) continue ;

                // First, are we an alt code
                if(sscanf(parts[sz], "{{%s}}", sub) == 1) {
                    if(!nullp(sub = alt_codes[sub])) {
                        parts[sz] = sub ;
                        continue ;
                    }
                }

                // Now, we have to be one of the colour codes!
                if(sscanf(parts[sz], "{{0%d}}", num) == 1) parts[sz] = fg_codes[num] ;
                else if(sscanf(parts[sz], "{{1%d}}", num) == 1) parts[sz] = bg_codes[num] ;

            }
            break ;
        case "low" :
            while(sz--) {
                // Skip non matches
                if(matched[sz] == 0) continue ;

                // First, are we an alt code
                if(sscanf(parts[sz], "{{%s}}", sub) == 1) {
                    if(!nullp(sub = alt_codes[sub])) {
                        parts[sz] = sub ;
                        continue ;
                    }
                }

                // Now, we have to be one of the colour codes!
                if(sscanf(parts[sz], "{{0%d}}", num) == 1) parts[sz] = high_to_low_fg[low_codes[num]] ;
                else if(sscanf(parts[sz], "{{1%d}}", num) == 1) parts[sz] = high_to_low_bg[low_codes[num]] ;
                else parts[sz] = "" ;
            }
            break ;
        default :
            return substitute_colour(text, "plain") ;
    }

    result = implode(parts, "") ;

    return result ;
}

/**
 * @daemon_function wrap
 * @description Wraps text to a specified width, preserving colour codes.
 * @param {string} str - The text to wrap.
 * @param {int} wrap_at - The column at which to wrap the text.
 * @param {int} indent_at - The number of spaces to indent wrapped lines.
 * @returns {string} The wrapped text.
 */
public string wrap(string str, int wrap_at, int indent_at) {
    string *sections ;

    sections = map(explode(str, "\n"), function(string section, int wrap_at, int indent_at) {
        string *parts = explode(section, " ");
        mapping running = ([ "length" : 0 ]);

        // this routine strips out the first space, put it back into the
        // array if the original string had a leading space
        if(section[0..0] == " ") parts = ({ "", parts... });

        parts = map(parts, function(string part, mapping running, int max, int indent) {
            string plain ;
            int len ;

            // new lines
            if(part[0..0] == "\n") {
                running["length"] = 0;
                return part ;
            }

            plain = substitute_colour(part, "plain") ;
            len = sizeof(plain);

            running["length"] += (len + 1);

            if(running["length"] >= max) {
                running["length"] = sizeof(plain) + indent;
                return "\n" + repeat_string(" ", indent) + part;
            }

            return part ;

        }, running, wrap_at, indent_at);

        return implode(map(explode(implode(parts, " "), "\n"), (: rtrim :)), "\n") ;
    }, wrap_at, indent_at) ;

    return implode(sections, "\n") ;
}

/**
 * @daemon_function colourp
 * @description Checks if a string contains colour codes.
 * @param {string} text - The text to check for colour codes.
 * @returns {int} 1 if colour codes are present, 0 otherwise.
 */
int colourp(string text) {
    return pcre_match(text, COLOUR_REGEX) ;
}

/**
 * @daemon_function resync
 * @description Reloads all colour definitions.
 */
void resync() {
    load_all_colours() ;
}

/**
 * @daemon_function token_to_colour
 * @description Converts a colour token to its corresponding colour code.
 * @param {string} token - The colour token to convert.
 * @returns {string} The corresponding colour code.
 */
string token_to_colour(string token) {
    if(pcre_match(token, "\\d{3}")) {
        int num ;

        sscanf(token, "%d", num) ;
        if(num < 0 || num > 255) return "" ;
// tell(this_body(), sprintf("FROM THE DAEMON: %d => %s\n", num, fg_codes[num][1..])) ;
        return fg_codes[num] ;
    } else {
        switch(token) {
            case "black"    : return high_to_low_fg[low_codes[0]] ;
            case "red"      : return high_to_low_fg[low_codes[1]] ;
            case "green"    : return high_to_low_fg[low_codes[2]] ;
            case "orange"   : return high_to_low_fg[low_codes[3]] ;
            case "yellow"   : return high_to_low_fg[low_codes[14]] ;
            case "blue"     : return high_to_low_fg[low_codes[4]] ;
            case "cyan"     : return high_to_low_fg[low_codes[5]] ;
            case "magenta"  : return high_to_low_fg[low_codes[6]] ;
            case "white"    : return high_to_low_fg[low_codes[7]] ;
            case "b_red"    : return high_to_low_fg[low_codes[9]] ;
            case "b_green"  : return high_to_low_fg[low_codes[10]] ;
            case "b_orange" : return high_to_low_fg[low_codes[14]] ;
            case "b_yellow" : return high_to_low_fg[low_codes[14]] ;
            case "b_blue"   : return high_to_low_fg[low_codes[11]] ;
            case "b_cyan"   : return high_to_low_fg[low_codes[12]] ;
            case "b_black"  : return high_to_low_fg[low_codes[13]] ;
            case "b_white"  : return high_to_low_fg[low_codes[14]] ;
            case "b_magenta": return high_to_low_fg[low_codes[15]] ;
            case "bold"     : return high_to_low_fg[low_codes[15]] ;
            case "reset"    : return "\e[0m" ;
            default: return "" ;
        }
    }

    return "" ;
}

/**
 * @daemon_function get_colour_list
 * @description Generates a formatted list of all available colours.
 * @returns {string} A formatted string containing all colour codes and their visual representation.
 */
string get_colour_list() {
    string output = "" ;
    int base ;
    int *colours = ({
        016,  017,  018,  019,  020,  021,  022,  023,  024,  025,  026,  027,
        028,  029,  030,  031,  032,  033,  034,  035,  036,  037,  038,  039,
        040,  041,  042,  043,  044,  045,  046,  047,  048,  049,  050,  051,
        082,  083,  084,  085,  086,  087,  076,  077,  078,  079,  080,  081,
        070,  071,  072,  073,  074,  075,  064,  065,  066,  067,  068,  069,
        058,  059,  060,  061,  062,  063,  052,  053,  054,  055,  056,  057,
        093,  092,  091,  090,  089,  088,  099,  098,  097,  096,  095,  094,
        105,  104,  103,  102,  101,  100,  111,  110,  109,  108,  107,  106,
        117,  116,  115,  114,  113,  112,  123,  122,  121,  120,  119,  118,
        159,  158,  157,  156,  155,  154,  153,  152,  151,  150,  149,  148,
        147,  146,  145,  144,  143,  142,  141,  140,  139,  138,  137,  136,
        135,  134,  133,  132,  131,  130,  129,  128,  127,  126,  125,  124,
        160,  161,  162,  163,  164,  165,  166,  167,  168,  169,  170,  171,
        172,  173,  174,  175,  176,  177,  178,  179,  180,  181,  182,  183,
        184,  185,  186,  187,  188,  189,  190,  191,  192,  193,  194,  195,
        226,  227,  228,  229,  230,  231,  220,  221,  222,  223,  224,  225,
        214,  215,  216,  217,  218,  219,  208,  209,  210,  211,  212,  213,
        202,  203,  204,  205,  206,  207,  196,  197,  198,  199,  200,  201,
    });

    // render as two 12 cell rows
    int *colours_greyscale = ({
        232,  233,  234,  235,  236,  237,  238,  239,  240,  241,  242,  243,
        255,  254,  253,  252,  251,  250,  249,  248,  247,  246,  245,  244,
    });

      // render as two 8 cell rows
    int *colours_low = ({
        000,  001,  002,  003,  004,  005,  006,  007,
        008,  009,  010,  011,  012,  013,  014,  015,
    });

    // Rainbow
    base = 16 ;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 6; j++) {
            for(int k = 0; k < 12; k++) {
                int colour = i*(6*12) + j + k*6 ;

                output += sprintf("  %s%'0'3d{{res}}",
                    sprintf("{{%'0'4d}}", colours[colour]),
                    colours[colour]
                ) ;
            }
            output += "\n";
        }
    }
    output += "\n";

    // Greyscale
    base = 232 ;
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 12; j++) {
            int colour = i*12 + j;

            output += sprintf("  %s%'0'3d{{res}}",
                sprintf("{{%'0'4d}}", colours_greyscale[colour]),
                colours_greyscale[colour]
            ) ;
        }
        output += "\n";
    }
    output += "\n";

    // Base colours
    base = 0 ;
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 8; j++) {
            int colour = i*8 + j;

            output += sprintf("  %s%'0'3d{{res}}",
                sprintf("{{%'0'4d}}", colours_low[colour]),
                colours_low[colour]
            );
        }
      output += "\n";
    }

    return output ;
}

/**
 * @daemon_function is_too_dark
 * @description Checks if a given colour is considered too dark.
 * @param {string} colour - The colour code to check.
 * @returns {int} 1 if the colour is too dark, 0 otherwise.
 */
int is_too_dark(string colour) {
    string *matches ;

    if(!too_dark_check())
        return 0 ;

    if(!pcre_match(colour, "^[0-1](?:1?[0-9]{1,2}|2[0-4][0-9]|25[0-5])$"))
        return 0 ;

    colour = colour[1..] ;
    colour = sprintf("%03d", to_int(colour)) ;
    if(too_dark_map()[colour]) return 1 ;
}

/**
 * @daemon_function substitute_too_dark
 * @description Substitutes a colour if it's considered too dark.
 * @param {string} text - The colour code to potentially substitute.
 * @returns {string} The substituted colour code if too dark, or the original if not.
 */
string substitute_too_dark(string text) {
    string *matches ;
    string result ;
    int num ;

    if(!too_dark_check())
        return text ;

    num = to_int(text) ;
    result = sprintf("%03d", num) ;

    if(too_dark_map()[result]) return too_dark_map()[result] ;
    else return text ;
}

/**
 * @daemon_function colour_to_greyscale
 * @description Converts a colour code to its greyscale equivalent.
 * @param {int} colour_code - The colour code to convert.
 * @returns {int} The greyscale equivalent of the colour code.
 */
int colour_to_greyscale(int colour_code) {
    float normalized ;
    int greyscale_code ;

    normalized = colour_code / 255.0;
    greyscale_code = to_int(normalized * 23) + 232;

    // Return the corresponding greyscale colour code
    return greyscale_code;
}

/**
 * @daemon_function body_colour_replace
 * @description Replaces colour codes in text based on body preferences and message type.
 * @param {object} body - The body object with colour preferences.
 * @param {string} text - The text to process.
 * @param {int} message_type - The type of message being processed.
 * @returns {string} The text with colours replaced according to preferences.
 */
public string body_colour_replace(object body, string text, int message_type) {
    int col ;
    string pref, colour ;

    if(!objectp(body) || !stringp(text) || !strlen(text) || !message_type)
        return text ;

    if(message_type & NO_COLOUR)
        return text ;
    else if(message_type & MSG_COMBAT_HIT)
        pref = body->query_pref("combat_hit_colour") ;
    else if(message_type & MSG_COMBAT_MISS)
        pref = body->query_pref("combat_miss_colour") ;
    else
        return text ;

    if(!stringp(pref) || !strlen(pref))
        return text ;

    col = to_int(pref);

    if(nullp(col))
        return text;

    colour = sprintf("%:4d", col);
    text = "{{" + colour + "}}" + text + "{{res}}";

    return text;
}

/**
 * @daemon_function colour_to_rgb
 * @description Converts a 256 colour code to an RGB tuple.
 * @param {int} colour_code - The 256 colour code (0-255).
 * @returns {int[]} An array containing the RGB values.
 */
public int *colour_to_rgb(int colour_code) {
    int r, g, b;

    if (colour_code < 0 || colour_code > 255) {
        return ({ 0, 0, 0 }); // Return black for out of range
    }

    if (colour_code < 16) {
        // Standard colors
        r = (colour_code & 1) ? 255 : 0;
        g = (colour_code & 2) ? 255 : 0;
        b = (colour_code & 4) ? 255 : 0;
        r += (colour_code & 8) ? 128 : 0;
    } else if (colour_code < 232) {
        // 6x6x6 color cube
        colour_code -= 16;
        r = (colour_code / 36) * 51;
        g = ((colour_code % 36) / 6) * 51;
        b = (colour_code % 6) * 51;
    } else {
        // Grayscale
        r = g = b = (colour_code - 232) * 10 + 8; // 232-255 maps to 8-255
    }

    return ({ r, g, b });
}

/**
 * @daemon_function rgb_to_colour
 * @description Converts RGB values to a 256 colour code.
 * @param {int} r - The red component (0-255).
 * @param {int} g - The green component (0-255).
 * @param {int} b - The blue component (0-255).
 * @returns {int} The corresponding 256 colour code.
 */
public int rgb_to_colour(int r, int g, int b) {
    if(r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        error("Invalid RGB values: " + r + ", " + g + ", " + b);

    if (r == g && g == b)
        // Grayscale
        return (r - 8) / 10 + 232; // 232-255 maps to 8-255
    else if (r >= 0 && r < 128 && g >= 0 && g < 128 && b >= 0 && b < 128)
        // Standard colors
        return (r > 0 ? 1 : 0) + (g > 0 ? 2 : 0) + (b > 0 ? 4 : 0);
    else {
        // 6x6x6 color cube
        int cube_r = r / 51;
        int cube_g = g / 51;
        int cube_b = b / 51;
        return 16 + (cube_r * 36) + (cube_g * 6) + cube_b;
    }
}

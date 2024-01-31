// /adm/daemons/xterm256_d.c
// XTerm Parser
//
// Created:     2022/08/22: Gesslar
// Last Change: 2022/08/22: Gesslar
//
// 2022/08/22: Gesslar - Created

#include <xterm256.h>

inherit DAEMON ;

private nosave string *fg_codes = ({ }) ;
private nosave string *bg_codes = ({ }) ;
private nosave mapping alt_codes = ([ ]) ;
private nosave string *fallback_codes = ({ }) ;
private nosave mapping x256_to_16_fg = ([ ]) ;
private nosave mapping x256_to_16_bg = ([ ]) ;

private void load_all_colours() ;
public string xterm256_wrap(string str, int wrap_at, int indent_at) ;
void resync() ;

void setup() {
    load_all_colours() ;
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

    lines = explode(read_file("/adm/etc/xterm256/256_to_16_fallback.txt"), "\n") ;
    i = 256 ;
    fallback_codes = allocate(i) ;
    while(i--) {
        string fallback ;

        sscanf(lines[i], "%*s,%s", fallback) ;

        fallback_codes[i] = fallback ;
    }

    lines = explode(read_file("/adm/etc/xterm256/xterm_ansi_16_fg.txt"), "\n") ;
    i = 16 ;
    while(i--) {
        string xterm ;
        string fallback ;

        sscanf(lines[i], "%s,%s", xterm, fallback) ;
        x256_to_16_fg[xterm] = sprintf("\e[%sm", fallback) ;
    }

    lines = explode(read_file("/adm/etc/xterm256/xterm_ansi_16_bg.txt"), "\n") ;
    i = 16 ;
    while(i--) {
        string xterm ;
        string fallback ;

        sscanf(lines[i], "%s,%s", xterm, fallback) ;
        x256_to_16_bg[xterm] = sprintf("\e[%sm", fallback) ;
    }
}

//:FUNCTION substitute_colour
// substitute_colour takes a string with tokenized xterm256 colour
// codes and a mode, parses the tokens and substitutes with
// xterm colour codes suitable for printing.
// available modes are:
// plain - strip all colour and style codes
// vt100 - strip only colour codes
// xterm - replace all tokens with xterm256 colour codes
// ansi  - fall back to ansi colour codes
public string substitute_colour(string text, string mode) {
    mixed *assoc ;
    string *parts, sub ;
    int *matched ;
    int sz, num ;
    string result ;

    if(nullp(text)) return "" ;
    if(nullp(mode)) mode = "plain" ;

    assoc = pcre_assoc(text, ({ XTERM256_COLOURS }), ({ 1 })) ;
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
                if(sscanf(parts[sz], "\e<%s>", sub) == 1) {
                    if(!nullp(sub = alt_codes[sub])) {
                        parts[sz] = sub ;
                        continue ;
                    }
                }

                parts[sz] = "" ;
            }
            break ;
        case "xterm" :
            while(sz--) {
                // Skip non matches
                if(matched[sz] == 0) continue ;

                // First, are we an alt code
                if(sscanf(parts[sz], "\e<%s>", sub) == 1) {
                    if(!nullp(sub = alt_codes[sub])) {
                        parts[sz] = sub ;
                        continue ;
                    }
                }

                // Now, we have to be one of the colour codes!
                if(sscanf(parts[sz], "\e<0%d>", num) == 1) parts[sz] = fg_codes[num] ;
                else if(sscanf(parts[sz], "\e<1%d>", num) == 1) parts[sz] = bg_codes[num] ;

            }
            break ;
        case "ansi" :
            while(sz--) {
                // Skip non matches
                if(matched[sz] == 0) continue ;

                // First, are we an alt code
                if(sscanf(parts[sz], "\e<%s>", sub) == 1) {
                    if(!nullp(sub = alt_codes[sub])) {
                        parts[sz] = sub ;
                        continue ;
                    }
                }

                // Now, we have to be one of the colour codes!
                if(sscanf(parts[sz], "\e<0%d>", num) == 1) parts[sz] = x256_to_16_fg[fallback_codes[num]] ;
                else if(sscanf(parts[sz], "\e<1%d>", num) == 1) parts[sz] = x256_to_16_bg[fallback_codes[num]] ;
                else parts[sz] = "" ;
            }
            break ;
        default :
            return substitute_colour(text, "plain") ;
    }

    result = implode(parts, "") ;

    return result ;
}

public string xterm256_wrap(string str, int wrap_at, int indent_at) {
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

int colorp(string text) {
    return pcre_match(text, XTERM256_COLOURS) ;
}

void resync() {
    load_all_colours() ;
}

string token_to_xterm(string token) {
    if(pcre_match(token, "\\d{3}")) {
        int num ;

        sscanf(token, "%d", num) ;
        if(num < 0 || num > 255) return "" ;
// tell(this_user(), sprintf("FROM THE DAEMON: %d => %s\n", num, fg_codes[num][1..])) ;
        return fg_codes[num] ;
    } else {
        switch(token) {
            case "black"    : return x256_to_16_fg[fallback_codes[0]] ;
            case "red"      : return x256_to_16_fg[fallback_codes[1]] ;
            case "green"    : return x256_to_16_fg[fallback_codes[2]] ;
            case "orange"   : return x256_to_16_fg[fallback_codes[3]] ;
            case "yellow"   : return x256_to_16_fg[fallback_codes[14]] ;
            case "blue"     : return x256_to_16_fg[fallback_codes[4]] ;
            case "cyan"     : return x256_to_16_fg[fallback_codes[5]] ;
            case "magenta"  : return x256_to_16_fg[fallback_codes[6]] ;
            case "white"    : return x256_to_16_fg[fallback_codes[7]] ;
            case "b_red"    : return x256_to_16_fg[fallback_codes[9]] ;
            case "b_green"  : return x256_to_16_fg[fallback_codes[10]] ;
            case "b_orange" : return x256_to_16_fg[fallback_codes[14]] ;
            case "b_yellow" : return x256_to_16_fg[fallback_codes[14]] ;
            case "b_blue"   : return x256_to_16_fg[fallback_codes[11]] ;
            case "b_cyan"   : return x256_to_16_fg[fallback_codes[12]] ;
            case "b_black"  : return x256_to_16_fg[fallback_codes[13]] ;
            case "b_white"  : return x256_to_16_fg[fallback_codes[14]] ;
            case "b_magenta": return x256_to_16_fg[fallback_codes[15]] ;
            case "bold"     : return x256_to_16_fg[fallback_codes[15]] ;
            case "reset"    : return "\e[0m" ;
            default: return "" ;
        }
    }

    return "" ;
}

string get_color_list() {
    string output = "" ;
    int base ;
    int *xterm256 = ({
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
    int *xterm_greyscale = ({
        232,  233,  234,  235,  236,  237,  238,  239,  240,  241,  242,  243,
        255,  254,  253,  252,  251,  250,  249,  248,  247,  246,  245,  244,
    });

      // render as two 8 cell rows
    int *xterm16 = ({
        000,  001,  002,  003,  004,  005,  006,  007,
        008,  009,  010,  011,  012,  013,  014,  015,
    });

    // Rainbow
    base = 16 ;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 6; j++) {
            for(int k = 0; k < 12; k++) {
                int color = i*(6*12) + j + k*6 ;

                output += sprintf("  %s%'0'4d\e[0m",
                    sprintf("\e[38;5;%dm", xterm256[color]),
                    xterm256[color]
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
            int color = i*12 + j;

            output += sprintf("  %s%'0'4d\e[0m",
                sprintf("\e[38;5;%dm", xterm_greyscale[color]),
                xterm_greyscale[color]
            ) ;
        }
        output += "\n";
    }
    output += "\n";

    // Base colors
    base = 0 ;
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 8; j++) {
            int color = i*8 + j;

            output += sprintf("  %s%'0'4d\e[0m",
                sprintf("\e[38;5;%dm", xterm16[color]),
                xterm16[color]
            );
        }
      output += "\n";
    }

    return output ;
}

string append(string source, string to_append) {
    int source_len, to_append_len;

    source_len = strlen(source);
    to_append_len = strlen(to_append);

    if(source[<to_append_len..] != to_append) {
        source += to_append;
    }

    return source;
}

string prepend(string source, string to_prepend) {
    int source_len, to_prepend_len;

    source_len = strlen(source);
    to_prepend_len = strlen(to_prepend);

    if(source[0..to_prepend_len-1] != to_prepend) {
        source = to_prepend + source;
    }

    return source;
}

varargs string extract(string str, int from, int to) {
    if(nullp(to)) return str[from ..];
    else return str[from .. to];
}

string no_ansi(string str) {
    return XTERM256->substitute_colour(str, "plain") ;
}

varargs string simple_list(string *arr, string conjunction) {
    if(!arr) error("simple_list: Missing argument 1 for simpple_list");
    if(!conjunction) conjunction = "and";

    if(sizeof(arr) == 0) error("simple_list: Argument 1 for simple_list is empty");
    else if(sizeof(arr) == 1) return arr[0];
    else if(sizeof(arr) == 2) return arr[0] + " " + conjunction + " " + arr[1];
    else return implode(arr[0..<2], ", ") + ", " + conjunction + " " + arr[<1];
}

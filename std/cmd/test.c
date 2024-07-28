// Test cases for header generation

inherit STD_OBJECT;

public int simple_function(int a, int b) {
    return a + b;
}

private string
multi_line_function(
    string arg1,
    int arg2
) {
    return arg1 + to_string(arg2);
}

mixed complex_return_type() {
    return ([ "key" : ({ 1, 2, 3 }) ]);
}

varargs void function_with_default_param(int a, int b : (: 10 :)) {
    write(sprintf("%d, %d", a, b));
}

protected void variadic_function(int a, int b...) {
    // Implementation
}

int function_with_inline_comment(int a /* first param */, int b /* second param */) {
    return a - b;
}

#ifdef DEBUG
void debug_function() {
    // Debug implementation
}
#endif

#if 0
void debug_function2() {
    // Debug implementation
}
#endif

/*
void debug_function3() {
    // Debug implementation
}
*/


// void function4() {
// // Implementation
// }

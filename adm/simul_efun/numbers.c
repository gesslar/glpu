#include "/adm/obj/simul_efun.h"

float percent_of(float a, float b) {
    return (to_float(a) / to_float(b)) * 100.0;
}

float percent(float a, float b) {
    return to_float(a) * 100.0 / to_float(b);
}

float range(float min, float max, float val) {
    if(val < min) return min;
    if(val > max) return max;
    return val;
}

varargs float remainder(mixed a, mixed b) {
    int result ;

    if(intp(a)) a = to_float(a);
    if(intp(b)) b = to_float(b);

    result = a / b ;

    return result - floor(result) ;
}

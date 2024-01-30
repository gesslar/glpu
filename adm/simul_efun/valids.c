#include <function.h>

int valid_function(mixed f) {
    int fp ;

    fp = functionp(f) ;

    if(fp) {
        return !(fp & FP_OWNER_DESTED) ;
    }

    return FALSE ;
}

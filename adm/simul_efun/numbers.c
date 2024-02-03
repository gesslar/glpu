#include "/adm/obj/simul_efun.h"

float percent_of(float a, float b) {
  return (to_float(a) / to_float(b)) * 100.0;
}

float percent(float a, float b) {
    return to_float(a) * 100.0 / to_float(b);
}

float range(float min, float max, float val) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

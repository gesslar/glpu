#include "/adm/obj/simul_efun.h"

float percent_of(float a, float b) {
  return (a / b) * 100.0;
}

float percent(float a, float b) {
    return a * 100.0 / b;
}

float range(float min, float max, float val) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

// /std/modules/currency.c
// Currency module
//
// Created:     2024/02/19: Gesslar
// Last Change: 2024/02/19: Gesslar
//
// 2024/02/19: Gesslar - Created

int convert_map_to_value(mapping currency_map) {
    mixed *conversion = mud_config("CURRENCY") ;
    int value = 0;

    foreach(mixed *currency in conversion) {
        if (currency_map[currency[0]]) {
            value += currency_map[currency[0]] * currency[1];
        }
    }
}

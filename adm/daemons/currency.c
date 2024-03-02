// /adm/daemons/currency.c
// Currency daemon
//
// Created:     2024/02/19: Gesslar
// Last Change: 2024/02/19: Gesslar
//
// 2024/02/19: Gesslar - Created

inherit STD_DAEMON ;

// Functions
int currency_index(string currency) ;

// Variables
mixed *currency_config ;

void setup() {
    currency_config = mud_config("CURRENCY") ;
}

int valid_currency_type(string currency) {
    return currency_index(currency) != -1 ;
}

int currency_index(string currency) {
    int i ;
    for (i = 0; i < sizeof(currency_config); i++) {
        if (currency_config[i][0] == currency) {
            return i ;
        }
    }
    return -1 ;
}

float convert_currency(int amount, string from_currency, string to_currency) {
    int from_index, to_index ;
    float from_rate, to_rate ;

    from_index = currency_index(from_currency) ;
    to_index = currency_index(to_currency) ;
    if (from_index == -1 || to_index == -1) {
        return -1 ;
    }

    from_rate = to_float(currency_config[from_index][1]) ;
    to_rate = to_float(currency_config[to_index][1]) ;
    return to_float(amount) * (from_rate / to_rate) ;
}

string lowest_currency() {
    int low = MAX_INT ;

    foreach (mixed *currency in currency_config) {
        if (currency[1] < low) {
            low = currency[1] ;
        }
    }

    return currency_config[low][0] ;
}

string highest_currency() {
    int high = 0 ;

    foreach (mixed *currency in currency_config) {
        if (currency[1] > high) {
            high = currency[1] ;
        }
    }

    return currency_config[high][0] ;
}

string *currency_list() {
    string *list ;
    mixed *currency ;

    currency = copy(currency_config) ;
    currency = sort_array(currency, (: $1[1] - $2[1] :) ) ;
    list = map(currency, (: $1[0] :) ) ;

    return list ;
}

// /std/modules/bank.c
//
// Created:     2024/02/29: Gesslar
// Last Change: 2024/02/29: Gesslar
//
// 2024/02/29: Gesslar - Created

#include <daemons.h>

void add_command(string cmd, string fun) ;

void init_bank() {
    add_command("register", "cmd_register");
    add_command("deposit", "cmd_deposit");
    add_command("withdraw", "cmd_withdraw");
    add_command("balance", "cmd_balance");
}

int cmd_balance(string arg) {
    object tp = this_player() ;
    mixed result ;

    result = BANK_D->query_balance(tp->query_name()) ;
    if(nullp(result)) {
        tell(tp, "You do not have an account with the bank.") ;
        return 1 ;
    }

    tell(tp, "You have " + add_commas(result) + " coins in your account.") ;
    return 1 ;
}

int cmd_register(string arg) {
    object tp = this_player() ;
    mixed result ;

    result = BANK_D->new_account(tp->query_name()) ;
    if(stringp(result)) {
        tell(tp, result) ;
        return 1 ;
    }

    if(result == 0) {
        tell(tp, "You already have an account with the bank.") ;
        return 1 ;
    }

    tell(tp, "You have successfully registered an account with the bank.") ;
    return 1 ;
}

int cmd_deposit(string str) {
    object tp = this_player() ;
    mixed *config = mud_config("CURRENCY") ;
    int num, have ;
    string type ;
    mixed result ;
    string name ;

    if(!str) {
        tell(tp, "Deposit what?") ;
        return 1 ;
    }

    if(sscanf(str, "%d %s", num, type) != 2) {
        tell(tp, "Syntax: deposit <number> <type>") ;
        return 1 ;
    }

    if(num < 1) {
        tell(tp, "You must deposit at least one coin.") ;
        return 1 ;
    }

    if(!CURRENCY_D->valid_currency_type(type)) {
        tell(tp, "That is not a valid currency type.") ;
        return 1 ;
    }

    name = tp->query_name() ;

    result = BANK_D->query_balance(name) ;
    if(nullp(result)) {
        tell(tp, "You do not have an account with the bank.") ;
        return 1 ;
    }

    have = tp->query_wealth(type) ;
    if(have < num) {
        tell(tp, "You do not have that many " + type + " coins.") ;
        return 1 ;
    }

    num = to_int(CURRENCY_D->convert_currency(num, type, "copper")) ;

    result = BANK_D->add_balance(name, num) ;
    if(stringp(result)) {
        tell(tp, result) ;
        return 1 ;
    }

    tp->add_wealth(type, -num) ;

    tell(tp, "You have deposited " + add_commas(num) + " " + type + " coins.") ;
    return 1 ;
}

int cmd_withdraw(string str) {
    object tp = this_player() ;
    mixed *config = mud_config("CURRENCY") ;
    int num, base, have, rate ;
    string type ;
    mixed result ;
    string name ;

    if(!str) {
        tell(tp, "Withdraw what?") ;
        return 1 ;
    }

    if(sscanf(str, "%d %s", num, type) != 2) {
        tell(tp, "Syntax: withdraw <number> <type>") ;
        return 1 ;
    }

    if(num < 1) {
        tell(tp, "You must withdraw at least one coin.") ;
        return 1 ;
    }

    if(!CURRENCY_D->valid_currency_type(type)) {
        tell(tp, "That is not a valid currency type.") ;
        return 1 ;
    }

    name = tp->query_name() ;

    result = BANK_D->query_balance(name) ;
    if(nullp(result)) {
        tell(tp, "You do not have an account with the bank.") ;
        return 1 ;
    }

    base = to_int(CURRENCY_D->convert_currency(num, type, "copper")) ;

    result = BANK_D->query_balance(name) ;
    if(base > result) {
        tell(tp, "You do not have that many coins in your account.") ;
        return 1 ;
    }

    result = BANK_D->add_balance(name, -base) ;
    if(stringp(result)) {
        tell(tp, result) ;
        return 1 ;
    }

    tp->add_wealth(type, num) ;

    tell(tp, "You have withdrawn " + add_commas(num) + " " + type + " coins.") ;
    return 1 ;
}

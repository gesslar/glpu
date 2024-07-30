#ifndef __WEALTH_H__
#define __WEALTH_H__

int query_total_coins();
int query_total_wealth();
mapping query_all_wealth();
int query_wealth(string currency);
int add_wealth(string currency, int amount);
void set_wealth(mapping w);
int can_carry_wealth(int amount);

#endif // __WEALTH_H__

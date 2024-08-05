#ifndef __WEALTH_H__
#define __WEALTH_H__

int query_total_coins();
int query_total_wealth();
mapping query_all_wealth();
int query_wealth(string currency);
int adjust_wealth(string currency, int amount);
void set_wealth(mapping w);
void wipe_wealth() ;

#endif // __WEALTH_H__

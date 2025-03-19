#ifndef __BOON_H__
#define __BOON_H__

public nomask void init_boon() ;
public nomask int boon(string name, string cl, string type, int amt, int dur) ;
public nomask int curse(string name, string cl, string type, int amt, int dur) ;
public nomask int query_boon(string cl, string type) ;
public mapping query_boon_data() ;
public nomask int query_curse(string cl, string type) ;
public mapping query_curse_data() ;
public nomask int query_effective_boon(string cl, string type) ;
protected nomask void process_boon() ;

#endif // __BOON_H__

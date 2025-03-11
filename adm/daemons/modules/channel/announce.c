/* chdmod_announce.c

 Tacitus @ LPUniversity
 28-JUNE-06
 Announce channel module

*/

inherit __DIR__ "channel" ;

void setup() {
  channel_names = ({ "announce" }) ;

  slot(SIG_USER_LOGIN, "announce_login") ;
  slot(SIG_USER_LOGOUT, "announce_logoff") ;
  slot(SIG_SYS_CRAWL_COMPLETE, "announce_crawl_complete") ;
}

int is_allowed(string channel, string name) {
  return 1 ;
}

int rec_msg(string chan, string usr, string msg) {
  object tp ;

  switch(msg) { /* We could do some neat stuff here! */
    case "/last" : {
      string *last_messages = last_messages(chan, 15) ;

      tp = find_player(usr) ;

      tell(tp, implode(last_messages, "\n")) ;

      return 1 ;
    }
  }

  if(!adminp(usr))
    return 0 ;

  CHAN_D->rec_msg(chan, lower_case(usr), sprintf("[%s] %s: %s\n", capitalize(chan), capitalize(usr), msg)) ;

  add_history("announce", sprintf("%s %s [%s] %s: %s\n",
    ldate(time(), 1),
    ltime(),
    capitalize(chan),
    capitalize(usr),
    msg
  )) ;

  return 1 ;
}

void announce_login(object user) {
  string name = capitalize(query_privs(user)) ;

  CHAN_D->rec_msg("announce", query_privs(user),
    sprintf("[Announce] System: %s has logged into %s.\n", name, mud_name())) ;

  add_history("announce",
    sprintf("%s %s [%s] %s: %s\n",
      ldate(time(),1),
      ltime(),
      capitalize(module_name),
      capitalize(name),
      "has logged into " + mud_name() + "."
    )
  ) ;
}

void announce_logoff(object user) {
  string name = capitalize(query_privs(user)) ;

  CHAN_D->rec_msg("announce", query_privs(user),
    sprintf("[Announce] System: %s has left %s.\n", name, mud_name())) ;

  add_history("announce",
    sprintf("%s %s [%s] %s: %s\n",
      ldate(time(),1),
      ltime(),
      capitalize(module_name),
      capitalize(name),
      "has left " + mud_name() + "."
    )
  ) ;
}

void announce_crawl_complete(mixed arg...) {
  string name = "System" ;

  CHAN_D->rec_msg("announce", "system",
    sprintf("[Announce] System: Coordinate crawler has completed.\n")) ;

  add_history("announce",
    sprintf("%s %s [%s] %s: %s\n",
      ldate(time(),1),
      ltime(),
      capitalize(module_name),
      capitalize(name),
      "Coordinate crawler has completed."
    )
  ) ;
}

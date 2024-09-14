/**
 * @file /adm/daemons/death.c
 * @description Death routines and functions
 *
 * @created 2024-07-28 - Gesslar
 * @last_modified 2024-07-28 - Gesslar
 *
 * @history
 * 2024-07-28 - Gesslar - Created
 */

inherit STD_DAEMON ;

void setup() {
  set_no_clean(1) ;
  slot(SIG_PLAYER_DIED, "player_died") ;
  slot(SIG_PLAYER_REVIVED, "player_revived") ;
}

void player_died(object player, object killer) {
  log_file("death", sprintf("[%s] %s was killed by %s [%O]\n",
      ctime(),
      player->query_name(),
      killer ? killer->query_name() : "unknown",
      environment(player)
    )
  ) ;
}

void player_revived(object player) {
  log_file("death", sprintf("[%s] %s was revived [%O]\n",
      ctime(),
      player->query_name(),
      environment(player)
    )
  ) ;
}

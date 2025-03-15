//lockdown_client.c

//Parthenon @ LPUniversity
//14-JUL-06
//Advanced lockdown command

//Last edited on July 17th, 2006 by Parthenon

inherit STD_CMD;

int main() {
    if(!load_object(LOCKDOWN_D)) {
        tell_me("\n Error [lockdown]: lockdown daemon not found\n");
            remove();
            return 1;
    }//END IF

    if(!adminp(query_privs(this_body()))) {
        tell_me("\n Error [lockdown]: access denied\n");
        remove();
        return 1;
    }//END IF

    tell_me("\n\tLOCKDOWN ADMIN TOOL\n\n");
    tell_me(" 1: Lockdown Mud\n");
    tell_me(" 2: IP Bans\n");
    tell_me(" 3: User Bans\n");
    tell_me(" 4: VIP users\n");
    tell_me(" 5: Quit\n");
    input_to("i_MainMenu");
    return 1;
}//END main()

int i_MainMenu(string arg) {
    if(!arg || arg == "") {
    tell_me("\n Error [lockdown]: invalid option\n");
    main();
    return 1;
    }//END IF

    switch(arg) {
        case "1":
            tell_me("\n\tLOCKDOWN MUD\n\n");

            if(LOCKDOWN_D->query_dev_lock()) tell_me(" 1: Lock developers out (active)\n");
            else tell_me(" 1: Lock developers out (inactive)\n");

            if(LOCKDOWN_D->query_player_lock()) tell_me(" 2: Lock players out (active)\n");
            else tell_me(" 2: Lock players out (inactive)\n");

            if(LOCKDOWN_D->query_vip_lock()) tell_me(" 3: Lock mud to all but VIP users (active)\n");
            else tell_me(" 3: Lock mud to all but VIP users (inactive)\n");

            if(LOCKDOWN_D->query_guest_locked()) tell_me(" 4: Enable guest account\n");
            else tell_me(" 4: Disable guest account\n");

            if(LOCKDOWN_D->query_creation_locked()) tell_me(" 5: Enable new player creation\n");
            else tell_me(" 5: Disable new player creation\n");

            tell_me(" 6: Unlock mud\n");
            tell_me(" 7: Main menu\n");
            input_to("i_LockChoice");

            return 1;
            break;
        case "2":
            tell_me("\n\tIP BANS\n\n");
            tell_me(" 1: Display IP bans\n");
            tell_me(" 2: Add IP ban\n");
            tell_me(" 3: Remove IP ban\n");
            tell_me(" 4: Remove all IP bans\n");
            tell_me(" 5: Main menu\n");
            input_to("i_IpBanChoice");
            break;
        case "3":
            tell_me("\n\tUSER BANS\n\n");
            tell_me(" 1: Display users banned\n");
            tell_me(" 2: Add user ban\n");
            tell_me(" 3: Remove user ban\n");
            tell_me(" 4: Remove all user bans\n");
            tell_me(" 5: Main menu\n");
            input_to("i_UserBanChoice");
            return 1;
            break;
        case "4":
            tell_me("\n\tVIP USERS\n\n");
            tell_me(" 1: Display all VIP users\n");
            tell_me(" 2: Add VIP user\n");
            tell_me(" 3: Remove VIP user\n");
            tell_me(" 4: Remove all VIP users\n");
            tell_me(" 5: Main menu\n");
            input_to("i_PlayTesterChoice");
            return 1;
            break;
        case "5":
            tell_me("\n Exiting lockdown tool...\n\n");
            remove();
            return 1;
            break;
        default:
            tell_me(" Error [lockdown]: invalid option\n");
            main();
            return 1;
            break;
    }//END SWITCH

    return 1;
}//END i_Main_Menu()

/***********************/
/* LOCK MENU FUNCTIONS */
/***********************/

int i_LockChoice(string arg) {
    if(!arg || arg == "") {
        tell_me("\n Error [lockdown]: invalid option\n");
        i_MainMenu("1");
        return 1;
    }//END IF

    switch(arg) {
        case "1":
            tell_me("\n Please enter the reason developers will be locked out or leave blank\n"
                "for the default message.\n");
            input_to("i_GetLockMessage", 0, arg);
            return 1;
            break;
        case "2":
            tell_me("\n Please enter the reason players will be locked out or leave blank\n"
                "for the default message.\n");
            input_to("i_GetLockMessage", 0, arg);
            return 1;
            break;
        case "3":
            tell_me("\n Please enter the reason the mud will be locked or leave blank\n"
                "for the default message.\n");
            input_to("i_GetLockMessage", 0, arg);
            return 1;
            break;
        case "4":
            if(LOCKDOWN_D->query_guest_locked()) {
                LOCKDOWN_D->toggle_guest_account();
                tell_me("\n Success [lockdown]: guest account successfully unlocked\n");
                i_MainMenu("1");
                return 1;
                break;
            }

            tell_me("\n Please enter the reason the guest account will be locked or leave blank\n"
                "for the default message.\n");
            input_to("i_GetLockMessage", 0, arg);
            break;
        case "5":
            if(LOCKDOWN_D->query_creation_locked()) {
                LOCKDOWN_D->toggle_new_player_creation();
                tell_me("\n Success [lockdown]: guest account successfully unlocked\n");
                i_MainMenu("1");
                return 1;
                break;
            }

            tell_me("\n Please enter the reason that new player creation will be locked or leave blank\n"
                "for the default message.\n");
            input_to("i_GetLockMessage", 0, arg);
            break;
        case "6":
            LOCKDOWN_D->unlock_mud();
            tell_me("\n Success [lockdown]: mud unlocked to everyone\n");
            i_MainMenu("1");
            return 1;
            break;
        case "7":
            tell_me("\n");
            main();
            return 1;
            break;
        default:
            tell_me("\n Error [lockdown]: invalid option\n");
            i_MainMenu("1");
            return 1;
            break;
    }//END SWITCH

    return 1;
}//END i_LockChoice

int i_GetLockMessage(string arg, string type) {
    if(!arg || arg == "") {
        switch(type) {
            case "1":
                LOCKDOWN_D->set_dev_lock();
                LOCKDOWN_D->set_dev_lock_msg("");
                tell_me("\n Success [lockdown]: developers now locked out. A default reason will be printed\n");
                i_MainMenu("1");
                return 1;
                break;
            case "2":
                LOCKDOWN_D->set_player_lock();
                LOCKDOWN_D->set_player_lock_msg("");
                tell_me("\n Success [lockdown]: players now locked out. A default reason will be printed\n");
                i_MainMenu("1");
                return 1;
                break;
            case "3":
                LOCKDOWN_D->set_vip_lock();
                LOCKDOWN_D->set_vip_lock_msg("");
                tell_me("\n Success [lockdown]: mud now locked to all but VIP players. A default reason will be printed\n");
                i_MainMenu("1");
                return 1;
                break;
            case "4":
                LOCKDOWN_D->toggle_guest_account();
                LOCKDOWN_D->set_guest_lock_msg("");
                tell_me("\n Success [lockdown]: guest account successfully locked. A default reason will be printed\n");
                i_MainMenu("1");
                return 1;
                break;
            case "5":
                LOCKDOWN_D->toggle_new_player_creation();
                LOCKDOWN_D->set_player_creation_msg("");
                tell_me("\n Success [lockdown]: new player creation successfully locked. A default reason will be printed\n");
                i_MainMenu("1");
                return 1;
                break;
        }//END SWITCH
    }//END IF

    switch(type) {
        case "1":
            LOCKDOWN_D->set_dev_lock();
            LOCKDOWN_D->set_dev_lock_msg(arg);
            tell_me("\n Success [lockdown]: developers now locked out. Your custom reason will be printed\n");
            i_MainMenu("1");
            return 1;
            break;
        case "2":
            LOCKDOWN_D->set_player_lock();
            LOCKDOWN_D->set_player_lock_msg(arg);
            tell_me("\n Success [lockdown]: players now locked out. Your custom reason will be printed\n");
            i_MainMenu("1");
            return 1;
            break;
        case "3":
            LOCKDOWN_D->set_vip_lock();
            LOCKDOWN_D->set_vip_lock_msg(arg);
            tell_me("\n Success [lockdown]: mud now locked to all but VIP players. Your custom reason will be printed\n");
            i_MainMenu("1");
            return 1;
            break;
        case "4":
            LOCKDOWN_D->toggle_guest_account();
            LOCKDOWN_D->set_guest_lock_msg(arg);
            tell_me("\n Success [lockdown]: guest account successfully locked. Your custom reason will be printed\n");
            i_MainMenu("1");
            return 1;
            break;
        case "5":
            LOCKDOWN_D->toggle_new_player_creation();
            LOCKDOWN_D->set_player_creation_msg(arg);
            tell_me("\n Success [lockdown]: new player creation successfully locked. Your custom reason will be printed\n");
            i_MainMenu("1");
            return 1;
            break;
    }//END SWITCH

    return 1;
}//END i_GetLockMessage

/********************/
/* IP BAN FUNCTIONS */
/********************/

int i_IpBanChoice(string arg) {
    int i;
    string output = "";

    string *banned_ips;

    if(!arg || arg == "") {
        tell_me("\n Error [lockdown]: invalid option\n");
        i_MainMenu("2");
        return 1;
    }//END IF

    switch(arg) {
        case "1":
            banned_ips = LOCKDOWN_D->query_banned_ips();

            if(!banned_ips || sizeof(banned_ips) < 1) {
                tell_me("\n There are no banned IP addresses at this time.\n");
                tell_me("\n [Hit enter to continue]\n");
                input_to("i_cnIpBan");
                return 1;
            }//END IF

            if(sizeof(banned_ips) == 1)
                output = banned_ips[0];
            else {
                for(i = 0; i < sizeof(banned_ips); i++) {
                    if(i == sizeof(banned_ips) - 1)
                        output += banned_ips[i];
                    else
                        output += banned_ips[i] + ", ";
                }//END FOR
            }//END IF

            tell_me("\n Currently banned IP addresses: \n");
            tell_me("\n " + output + "\n");
            tell_me("\n [Hit enter to continue]\n");
            input_to("i_cnIpBan");
            return 1;
            break;
        case "2":
            tell_me("\n Please enter the IP address you wish to ban (x.x.x.x):\n"+
                " (Note that if you wish, you may place the wildcard '*' as any\n"+
                " part of the IP address, e.g. *.222.*.1)\n");
            input_to("i_GetIpAddress", 0, 1);
            return 1;
            break;
        case "3":
            tell_me("\n Please enter the IP address you wish to unban (x.x.x.x): ");
            input_to("i_GetIpAddress", 0, 2);
            return 1;
            break;
        case "4":
            tell_me("\n Are you sure you wish to remove all IP address bans?\n");
            input_to("i_ConRemoveAllIpBans");
            return 1;
            break;
        case "5":
            tell_me("\n");
            main();
            return 1;
        default:
            tell_me("\n Error [lockdown]: invalid option\n");
            i_MainMenu("2");
            return 1;
            break;
    }//END SWITCH

    return 1;
}//END i_IpBanChoice

int i_GetIpAddress(string arg, int state) {
    string first, second, third, fourth;
    string ip_item = "^[0-9]?[0-9]?[0-9]?$";
    mixed val;

    if(!arg || arg == "") {
        tell_me("\n Error [lockdown]: invalid IP address\n");
        i_MainMenu("2");
        return 1;
    }//END IF

    if(sscanf(arg, "%s.%s.%s.%s", first, second, third, fourth) != 4) {
        tell_me("\n Error [lockdown]: invalid IP address\n");
        i_MainMenu("2");
        return 1;
    }//END IF

    if((!regexp(first, ip_item) && first != "*") ||
      (!regexp(second, ip_item) && second != "*") ||
      (!regexp(third, ip_item) && third != "*") ||
      (!regexp(fourth, ip_item) && fourth != "*")) {
        tell_me("\n Error [lockdown]: invalid IP address\n");
        i_MainMenu("2");
        return 1;
    }//END IF

    if(state == 1)
        val = LOCKDOWN_D->add_ip_ban(arg);
    else if(state == 2)
        val = LOCKDOWN_D->remove_ip_ban(arg);

    if(stringp(val)) {
        tell_me("\n Error [lockdown]: " + val + "\n");
        i_MainMenu("2");
        return 1;
    } else {
        if(state == 1)
            tell_me("\n Success [lockdown]: IP address " + first + "." + second + "." + third + "." + fourth + " successfully banned\n");
        else if(state == 2)
            tell_me("\n Success [lockdown]: IP address " + first + "." + second + "." + third + "." + fourth + " successfully unbanned\n");
    }//END IF

    i_MainMenu("2");
    return 1;
}//END i_GetIpAddress

int i_ConRemoveAllIpBans(string arg) {
    if(!arg || arg == "" || (member_array(lower_case(arg), ({ "n", "y", "no", "yes" })) == -1)) {
        tell_me("\n Error [lockdown]: invalid option\n");
        i_MainMenu("2");
        return 1;
    }//END IF

    if(lower_case(arg) == "n" || lower_case(arg) == "no") {
        tell_me("\n Cancelling removal of all IP address bans...\n");
        i_MainMenu("2");
        return 1;
    }//END IF

    if(!LOCKDOWN_D->remove_all_ip_bans()) {
        tell_me("\n Error [lockdown]: IP addresses bans could not be removed\n");
        i_MainMenu("2");
        return 1;
    }//END IF

    tell_me("\n Success [lockdown]: all IP address bans have been removed\n");
    i_MainMenu("2");
    return 1;
}//END i_ConRemoveAllIpBans

int i_cnIpBan(string arg) {
    i_MainMenu("2");
    return 1;
}//END i_cnIpBan

/**********************/
/* USER BAN FUNCTIONS */
/**********************/

int i_UserBanChoice(string arg) {
    int i;
    string *banned_users;
    string output = "";

    if(!arg || arg == "") {
        tell_me("\n Error [lockdown]: invalid option\n");
        i_MainMenu("3");
        return 1;
    }//END IF

    switch(arg) {
        case "1":
            banned_users = LOCKDOWN_D->query_banned_users();

            if(!banned_users || sizeof(banned_users) < 1) {
                tell_me("\n There are no banned users at this time.\n");
                tell_me("\n [Hit enter to continue]\n");
                input_to("i_cnUserBan");
                return 1;
            }//END IF

            if(sizeof(banned_users) == 1)
                output = capitalize(banned_users[0]);
            else {
                for(i = 0; i < sizeof(banned_users); i++) {
                    if(i == sizeof(banned_users) - 1)
                        output += capitalize(banned_users[i]);
                    else
                        output += capitalize(banned_users[i]) + ", ";
                }//END FOR
            }//END IF

            tell_me("\n Currently banned usernames: \n");
            tell_me("\n " + output + "\n");
            tell_me("\n [Hit enter to continue]\n");
            input_to("i_cnUserBan");
            return 1;
            break;
        case "2":
            tell_me("\n Please enter the name of the user you wish to ban:\n" +
                  " (Note that this does not need to actually be a username, if you\n"+
                  " wish what you enter to match any part of a name such as a cuss\n"+
                  " word, then you may put a '*' on either side of it)\n");
            input_to("i_GetUserName", 0, 1);
            return 1;
            break;
        case "3":
            tell_me("\n Please enter the name of the user you wish to unban:\n");
            input_to("i_GetUserName", 0, 2);
            return 1;
            break;
        case "4":
            tell_me("\n Are you sure you wish to remove all user bans?\n");
            input_to("i_ConRemoveAllUserBans");
            return 1;
            break;
        case "5":
            tell_me("\n");
            main();
            return 1;
        default:
            tell_me("\n Error [lockdown]: invalid option\n");
            i_MainMenu("3");
            return 1;
            break;
    }//END SWITCH

    return 1;
}//END i_UserBanChoice

varargs int i_GetUserName(string arg, int state, int confirm) {
    mixed val;
    string tmp;

    if(!arg || arg == "") {
        tell_me("\n Error [lockdown]: invalid username\n");
        if(state == 1)
            i_UserBanChoice("2");
        else if(state == 2)
            i_UserBanChoice("3");
        return 1;
    }//END IF

    tmp = arg;

    if(tmp[0] == '*' && tmp[<1] == '*')
    arg = ".*" + arg[1..<2] + ".*";

    if(!ofile_exists(user_data_file(arg)) && state == 1 && !confirm) {
        tell_me("\n That user does not exist... do you still wish to add it to\n"+
            "the ban list?\n");
        input_to("i_ConfirmUserBan", arg);
        return 1;
    }//END IF

    if(state == 1)
        val = LOCKDOWN_D->add_banned_user(arg);
    else if(state == 2)
        val = LOCKDOWN_D->remove_banned_user(arg);

    if(stringp(val)) {
        tell_me("\n Error [lockdown]: " + val + "\n");
        i_MainMenu("3");
        return 1;
    } else {
        if(state == 1)
            tell_me("\n Success [lockdown]: user " + capitalize(arg) + " successfully banned\n");
        else if(state == 2)
            tell_me("\n Success [lockdown]: user " + capitalize(arg) + " succesfully unbanned\n");
    }//END IF

    i_MainMenu("3");
    return 1;
}//END i_GetUserName

int i_ConfirmUserBan(string arg, string name) {
    if(!arg || arg == "") {
        tell_me("\n Error [lockdown]: invalid option\n");
        input_to("i_ConfirmUserBan");
        return 1;
    }//END IF

    if(lower_case(arg) == "yes" || lower_case(arg) == "y")
        i_GetUserName(name, 1, 1);
    else
        i_MainMenu("3");

    return 1;
}//END i_ConfirmUserBan

int i_cnUserBan(string arg) {
    i_MainMenu("3");
    return 1;
}//END i_cnUserBan

int i_ConRemoveAllUserBans(string arg) {
    if(!arg || arg == "" || (member_array(lower_case(arg), ({ "n", "y", "no", "yes" })) == -1)) {
        tell_me("\n Error [lockdown]: invalid option\n");
        i_UserBanChoice("4");
        return 1;
    }//END IF

    if(lower_case(arg) == "n" || lower_case(arg) == "no") {
        tell_me("\n Cancelling removal of all user bans...\n");
        i_MainMenu("3");
        return 1;
    }//END IF

    if(!LOCKDOWN_D->remove_all_banned_users()) {
        tell_me("\n Error [lockdown]: user bans could not be removed\n");
        i_MainMenu("3");
        return 1;
    }//END IF

    tell_me("\n Success [lockdown]: all user bans have been removed\n");
    i_MainMenu("3");
    return 1;
}//END i_ConRemoveAllUserBans

/*********************/
/* VIP BAN FUNCTIONS */
/*********************/

int i_PlayTesterChoice(string arg) {
    string *play_testers;
    int i;
    string output = "";

    if(!arg || arg == "") {
        tell_me("\n Error [lockdown]: invalid option\n");
        i_MainMenu("4");
        return 1;
    }//END IF

    switch(arg) {
        case "1":
            play_testers = LOCKDOWN_D->query_play_testers();

            if(!play_testers || sizeof(play_testers) < 1) {
                tell_me("\n There are no VIP users at this time.\n");
                tell_me("\n [Hit enter to continue]\n");
                input_to("i_cnPlayTest");
                return 1;
            }//END IF

            if(sizeof(play_testers) == 1)
                output = capitalize(play_testers[0]);
            else {
                for(i = 0; i < sizeof(play_testers); i++) {
                    if(i == sizeof(play_testers) - 1)
                        output += capitalize(play_testers[i]);
                    else
                        output += capitalize(play_testers[i]) + ", ";
                }//END FOR
            }//END IF

            tell_me("\n Current VIP users: \n");
            tell_me("\n " + output + "\n");
            tell_me("\n [Hit enter to continue]\n");
            input_to("i_cnPlayTest");
            return 1;
            break;
        case "2":
            tell_me("\n Please enter the name of the user you wish to add as a VIP user: ");
            input_to("i_GetPlayTester", 0, 1);
            return 1;
            break;
        case "3":
            tell_me("\n Please enter the name of the user you wish to remove as a VIP user: ");
            input_to("i_GetPlayTester", 0, 2);
            return 1;
            break;
        case "4":
            tell_me("\n Are you sure you wish to remove all VIP users?\n");
            input_to("i_ConRemoveAllPlayTesters");
            return 1;
            break;
        case "5":
            tell_me("\n");
            main();
            return 1;
        default:
            tell_me("\n Error [lockdown]: invalid option\n");
            i_MainMenu("4");
            return 1;
            break;
    }//END SWITCH

    return 1;
}//END i_PlayTesterChoice

int i_ConRemoveAllPlayTesters(string arg) {
    if(!arg || arg == "" || (member_array(lower_case(arg), ({ "n", "y", "no", "yes" })) == -1)) {
        tell_me("\n Error [lockdown]: invalid option\n");
        i_PlayTesterChoice("4");
        return 1;
    }//END IF

    if(lower_case(arg) == "n" || lower_case(arg) == "no") {
        tell_me("\n Cancelling removal of all VIP users...\n");
        i_MainMenu("4");
        return 1;
    }//END IF

    if(!LOCKDOWN_D->remove_all_play_testers()) {
        tell_me("\n Error [lockdown]: VIP users could not be removed\n");
        i_MainMenu("4");
        return 1;
    }//END IF

    tell_me("\n Success [lockdown]: all VIP users have been removed\n");
    i_MainMenu("4");
    return 1;
}//END i_ConRemoveAllPlayTesters

varargs int i_GetPlayTester(string arg, int state, int confirm) {
    mixed val;

    if(!arg || arg == "") {
        tell_me("\n Error [lockdown]: invalid username\n");
        if(state == 1)
            i_PlayTesterChoice("2");
        else if(state == 2)
            i_PlayTesterChoice("3");
        return 1;
    }//END IF

    if(!ofile_exists(user_data_file(arg)) && state == 1 && !confirm) {
        tell_me("\n That user does not exist... do you still wish to add it to\n"+
            "the VIP user list?\n");
        input_to("i_ConfirmPlayTesterAdd", arg);
        return 1;
    }//END IF

    if(state == 1)
        val = LOCKDOWN_D->add_play_tester(arg);
    else if(state == 2)
        val = LOCKDOWN_D->remove_play_tester(arg);

    if(stringp(val)) {
        tell_me("\n Error [lockdown]: " + val + "\n");
        i_MainMenu("4");
        return 1;
    } else {
        if(state == 1)
            tell_me("\n Success [lockdown]: user " + capitalize(arg) + " successfully added as a VIP user\n");
        else if(state == 2)
            tell_me("\n Success [lockdown]: user " + capitalize(arg) + " succesfully removed as a VIP user\n");
    }//END IF

    i_MainMenu("4");
    return 1;
}//END i_GetPlayTester

int i_ConfirmPlayTesterAdd(string arg, string name) {
    if(!arg || arg == "") {
        tell_me("\n Error [lockdown]: invalid option\n");
        input_to("i_ConfirmPlayTesterAdd");
        return 1;
    }//END IF

    if(lower_case(arg) == "yes" || lower_case(arg) == "y")
        i_GetPlayTester(name, 1, 1);
    else
        i_MainMenu("4");

    return 1;
}//END i_ConfirmPlayTesterAdd

int i_cnPlayTest(string arg) {
    i_MainMenu("4");
    return 1;
}//END i_cnPlayTest

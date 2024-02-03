# LPUniversity Mudlib #

The LPUniversity Mudlib was downloaded from http://dead-souls.net/files/lpuni_fluffos_v1.zip and converted and fixed to work with the current version of FluffOS 2019 before being posted to this git repository.

## FluffOS ##

* FluffOS can be downloaded from Github at https://github.com/fluffos/fluffos
* FluffOS build instructions: https://www.fluffos.info/build.html

## Special notes ##

* This mudlib requires that the driver be compiled with `-DPACKAGE_UIDS=OFF`
* All original documentation, licenses, copyright information has been moved to `/ARCHIVE`.
* The first user to login will become admin. This is due to the absence of `/adm/etc/first_user` file. This file is created following the creation of the first user.
* Default in-game configurations are found in `/adm/etc/default.json`. You may provide overrides or new values in `/adm/etc/config.json`, does not exist by default, nor does it get watched by git.

## Runtime Config File ##
A sample config file has been supplied in `/adm/etc/config.mud`. You will want to configure the following to get the game up and running:

```
# name of this mud
name : LPUni

# port number to accept users on
port number : 1337

# absolute pathname of mudlib
mudlib directory : /path/to/mudlib

# debug.log and author/domain stats are stored here
# this path is relative to the operating system and you should
# make it point your log directory if you would like your debug.log
# file to be inside the lib
log directory : /path/to/mudlib/log
```

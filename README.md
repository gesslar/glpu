# LPUniversity Mudlib #

The LPUniversity Mudlib was downloaded from http://lpmuds.net/downloads.html and converted and fixed to work with the current version of FluffOS 2019 before being posted to this git repository.

## FluffOS ##

* FluffOS can be downloaded from Github at https://github.com/fluffos/fluffos
* FluffOS build instructions: https://www.fluffos.info/build.html

## Special notes ##

* This mudlib requires that the driver be compiled with `-DPACKAGE_UIDS=OFF`
* All original documentation, licenses, copyright information has been moved to `/ARCHIVE`.
* The first user to login will become admin. This is due to the existence of `/adm/etc/new_install` file. This file is removed following the creation of the first user.

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

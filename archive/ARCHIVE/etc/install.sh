#!/bin/sh

# Tacitus @ LPUniversity
# 15-APRIL-2006
# LPUniversity Mudlib Install Script

#define colours
export CRE="[K"
export NORMAL="[0;39m"
export RED="[1;31m"
export GREEN="[1;32m"
export YELLOW="[1;33m"
export BLUE="[1;34m"
export MAGENTA="[1;35m"
export CYAN="[1;36m"
export WHITE="[1;37m"
export SAVE="[s"
export RESTORE="[u"
export CLEAR="[K"
export RESET="[2J"

echo "=--------------------------------------------=";
echo "|${BLUE} Welcome to the LPUniversity Install Script${NORMAL} |";
echo "=--------------------------------------------=";
echo;
echo "This script will automatically install and configure";
echo "the LPUniversity mudlib after getting some information";
echo "from you. If you do not wish to continue with this process";
echo "then please exit from this script now by pressing the";
echo "button labelled 'ctrl' and the letter 'c' at the same time.";
echo;
echo "Hit any key to continue...";
read blah;

while(true)
do
echo;
echo "${CYAN}Step 1: Preliminary Questions${NORMAL}"
echo;
echo "We now need to find out where you'd like to install LPUniversity.";
echo "If you do not provide an argument, then it will install to the current directory.";
echo;
while(true)
do

echo -n "Install to [./mud/]: ";

read install_dir;
expr "$install_dir" == "" >/dev/null && install_dir="$(pwd)/mud";
[ -d "$install_dir" ] && break
echo;
echo "${RED} The directory '$install_dir' does not exist.${NORMAL}";
mkdir $install_dir;
if test -d $install_dir
	then
		echo "${GREEN} The directory '$install_dir' has been created.${NORMAL}";
		break;
	else
		echo "Please try again.";
fi

done

echo;
echo "${CYAN}Base install directory set to:${NORMAL}";
echo " $install_dir";
echo;
echo "${CYAN}The executables will be installed to:${NORMAL}";
echo " $install_dir/bin/";
echo;
echo "${CYAN}Scripts and package documentation will be installed to:${NORMAL}";
echo " $install_dir/etc/";
echo;
echo "${CYAN}The source for the MudOS driver will be installed to:${NORMAL}";
echo " $install_dir/source/";
echo;
echo "${CYAN}The mudlib will be installed to:${NORMAL}";
echo " $install_dir/lib/";
echo;
echo -n "Is this acceptable? Please respond with either 'y' for yes or 'n' for no: ";
read confirm_dir;
expr "$confirm_dir" == "y" >/dev/null && break;
done

while(true)
do

echo;
echo "${YELLOW}What would you like to call your mud? You may change this later.${NORMAL}";
echo -n "Please enter the name of your mud: ";
read mud_name;
echo "${RED}Your mud will be called: $mud_name${NORMAL}";
echo;

echo "${YELLOW}What port would you like your mud to run on?${NORMAL}";
echo -n "Enter port: "
read port;
echo "${RED}Your mud will run on port $port${NORMAL}";
echo;

echo "An address server allows you to resolve ip addresses to hostnames.";
echo "${YELLOW}What port would you like the address server to run on?${NORMAL}";
echo -n "Enter port for address server: ";
read addr_port;
echo "${RED}Your mud's address server will run on port $addr_port${NORMAL}";
echo;
echo;

echo "${YELLOW}Do you want the script to compile MudOS for you?${NORMAL}";
echo "${RED}This is NOT required if you are running linux.${NORMAL}";
echo -n "Compile MudOS? [y]es or [n]o?: ";
read compile_mudos;

if (test $compile_mudos == "y" || test $compile_mudos == "yes" || test $compile_mudos == "Yes" || test $compile_mudos == "y");
	then
		compile_mudos="Yes";
		echo "${RED}This install script will recompile MudOS for you.${NORMAL}";
	else
		compile_mudos="No";
		echo "${RED}This script will NOT recompile MudOS for you.${NORMAL}";
fi

echo;
echo "${RED}Please confirm that the following information is correct:${NORMAL}";
echo;
echo "${BLUE}Mud name${NORMAL}             : $mud_name";
echo "${BLUE}Mud Port${NORMAL}             : $port";
echo "${BLUE}Address Server Port${NORMAL}  : $addr_port";
echo "${BLUE}Compile Mudos${NORMAL}        : $compile_mudos";
echo;
echo -n "Are the above options correct? [y]es or [n]o: ";

read confirm_install;
expr "$confirm_install" == "y" >/dev/null && break;
done

echo;
echo "${CYAN}Step 2: Unpacking and generating config files.${NORMAL}";
echo;

[ -f "$(pwd)/lpuni.tar.gz" ] || {
	echo;
	echo "${RED}Error${NORMAL} [install_script]: Archive not found. Please redownload package.";
	exit 0
}

tar -C $install_dir -xzf $(pwd)/lpuni.tar.gz
if test $? != 0
	then
		echo "${RED}Error${NORMAL} [install_script]: Unpacking the archive failed. Please redownload package.";
		exit $?;
	else
		echo "${GREEN}Success${NORMAL} [install_script]: Unpacking the archive was successful.";
fi

echo;
echo "Generating config files...";
echo;

echo "###############################################################################
#                LPUniversity Mudlib Runtime Configuation File                #
###############################################################################
# NOTE: All paths specified here are relative to the mudlib directory except  #
#       for mudlib directory, and binary directory.                           #
# Lines beginning with a # or a newline are ignored.                          #
###############################################################################

# name of this mud
name : $mud_name

# port number to accept users on
port number : $port

# the address server is an external program that lets the mud translate
# internet names to numbers (and vice versa).  select an unused port.
address server ip : localhost
address server port : $addr_port

# absolute pathname of mudlib
mudlib directory : $install_dir/lib/

# absolute pathname of driver/config dir
binary directory : $install_dir/bin/

# debug.log and author/domain stats are stored here
log directory : /log

# the directories which are searched by #include <...>
# for multiple dirs, separate each path with a ':'
include directories : /adm/include:/include

# Directory to save binaries in.  (if BINARIES is defined)
save binaries directory : /adm/temp/

# the file which defines the master object
master file : /adm/obj/master

# the file where all global simulated efuns are defined.
simulated efun file : /adm/obj/simul_efun

# file to swap out objects; not used if time to swap is 0
swap file : /adm/tmp/swapfile

# alternate debug.log file name (assumed to be in specified 'log directory')
debug log file : debug.log

# This is an include file which is automatically #include'd in all objects
global include file : <global.h>

# if an object is left alone for a certain time, then the
# function clean_up will be called. This function can do anything,
# like destructing the object. If the function isn't defined by the
# object, then nothing will happen.
# This time should be substantially longer than the swapping time.
time to clean up : 120000

# How long time until an unused object is swapped out.
# Machine with too many players and too little memory: 900 (15 minutes)
# Machine with few players and lot of memory: 10000
# Machine with infinite memory: 0 (never swap).
time to swap : 10000

# How many seconds until an object is reset again.
time to reset : 1800

# Maximum number of bits in a bit field. They are stored in printable
# strings, 6 bits per byte.
maximum bits in a bitfield : 1200

# Max number of local variables in a function.
maximum local variables : 30

# Maximum amount of 'eval cost' per thread - execution is halted when
# it is exceeded.
maximum evaluation cost : 500000

# This is the maximum array size allowed for one single array.
maximum array size : 15000

# This is the maximum allowed size of a variable of type 'buffer'.
maximum buffer size : 400000

# Max size for a mapping
maximum mapping size : 1500000

# Max inherit chain size
inherit chain size : 30

# maximum length of a string variable
maximum string length : 200000

# Max size of a file allowed to be read by 'read_file()'.
maximum read file size : 200000

# max number of bytes you allow to be read and written with read_bytes
# and write_bytes
maximum byte transfer : 10000

# Reserve an extra memory area from malloc(), to free when we run out
# of memory and allow the mudlib to shutdown.
# If this value is 0, no area will be reserved.
reserved size : 0

# Define the size of the shared string hash table.  This number should
# a prime, probably between 1000 and 30000; if you set it to about 1/5
# of the number of distinct strings you have, you will get a hit ratio
# (number of comparisons to find a string) very close to 1, as found strings
# are automatically moved to the head of a hash chain.  You will never
# need more, and you will still get good results with a smaller table.
hash table size : 7001

# Object hash table size.
# Define this like you did with the strings; probably set to about 1/4 of
# the number of objects in a game, as the distribution of accesses to
# objects is somewhat more uniform than that of strings.
object table size : 1501

# default no-matching-action message
default fail message : What?

# default message when error() occurs (optional)
default error message :

###############################################################################
#          The following aren't currently used or implemented (yet)           #
###############################################################################

# maximum number of users in the game (unused currently)
maximum users : 40

# Define the maximum stack size of the stack machine. This stack will also
# contain all local variables and arguments.  (unused currently)
evaluator stack size : 1000

# Define the size of the compiler stack. This defines how complex
# expressions the compiler can parse.  (unused currently)
compiler stack size : 200

# Define the maximum call depth for functions. (unused currently)
maximum call depth : 30

# There is a hash table for living objects, used by find_living().
# (unused currently)
living hash table size : 100" > "$install_dir/etc/config.lpu";

echo "#!/bin/sh

#
# file:    startmud
# version: 1.0
# date:    08-OCT-05
# desc:    Startmud script based off of TMI-II Script
#
#

# WARNING: After starting script for the first time, ensure
#     that the script is not caught in a loop writing error
#     messages to startmud.err

# NOTE: I've decided to use relative directories so that you
#     might not even need to modify this script to get started.
#     If it doesn't work on your system, just replace with full
#     paths (You will however have to modify the paths in
#     config.lpu).

# -=CONFIGURATION=-

#  BINDIR
# Directory of driver binaries
# BINDIR=/users/somervil/lpuni/bin
BINDIR=../bin

#  ETCDIR
# Directory with runtime configuration data
# ETCDIR=/users/somervil/lpuni/etc
ETCDIR=./

#  LOGDIR
# Mudlib directory containing driver-related log fiels
# LOGDIR=/users/somervil/tmi3/lib/log/driver
LOGDIR=../lib/log/driver


#  DRIVER
# Name of driver binary (default: driver)
DRIVER=driver

#  ADDR_SERVER
# Name of address server binary (default: addr_server)
ASERVER=addr_server

#  APORT
# Port that the mudlib will look for the address server on
APORT=$addr_port

#  CONFIG
# MudOS runtime configuration file
CONFIG=config.lpu

#  UP
# Name of logfile showing dates when driver starts up and goes down
UP=UPRECORD

# LOG
# Name of driver logfile
LOG=driver.log

#  ERR
# Name of driver logfile for output sent to stderr
ERR=driver.err

#  ALOG
# Name of address server stdout server log file
ALOG=aserver.log

#  AERR
# Name of address server stderr log file
AERR=aserver.err
`cat $install_dir/etc/startmud.part`" > "$install_dir/etc/startmud";

rm "$install_dir/etc/startmud.part";
chmod +x "$install_dir/etc/startmud";
chmod -x "$install_dir/etc/install.sh";
touch "$install_dir/lib/adm/etc/new_install";

if test $compile_mudos == "Yes"
	then
		echo "Attempting to compile MudOS...";
		echo;
		echo "Backing up existing binaries...";
		echo;
		mv "$install_dir/bin/driver" "$install_dir/bin/driver.bk";
		mv "$install_dir/bin/addr_server" "$install_dir/bin/addr_server.bk";
		mv "$install_dir/bin/portbind" "$install_dir/bin/portbind.bk";
		echo "Starting compile...";
		echo;
		cd "$install_dir/source/";
		make clean;
		./build.MudOS || { echo "Error compiling Mudos."; };
		make || { echo "Error compiling Mudos."; };
		make install || { echo "Error compiling Mudos."; };
		echo;
		echo "Success!";
		echo;
	else
		echo;
		echo "Success!";
		echo;
fi

echo "${CYAN}Step 3: Conclusion${NORMAL}";

echo;
echo "${GREEN}Success${NORMAL} [install_script]: $mud_name is now setup.";
echo;

echo "You can startmud the mud by typing: cd $install_dir/etc/";
echo "Then typing: ./startmud &";
echo "If there are any problems, it will be written to startmud.log and startmud.err";
echo "If the problem is with the mudlib, then the errors will be written to";
echo "log files in $startmud/lib/log/driver/";
echo;
echo "${RED}After the mud has succesfully booted, log in as the first new user";
echo "will become admin as soon as they login.${NORMAL}";
echo;
echo "For further assistance and an excellent LP-centric community,";
echo "please visit http://lpuni.sourceforge.net";

echo;
echo "Bug reports, feature requests, support requests, and patchs can be";
echo "submitted to http://sourceforge.net/projects/lpuni";
echo;
echo "Thank you installing LPUniversity.";

echo;
echo "${GREEN}Success [install_script]: Script complete"${NORMAL};
echo;

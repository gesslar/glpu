# gLPU (LPUniversity)

Thank you for your interest in gLPU (Gesslar's LPUniversity) which is a fork
of LPUniversity.

## Introduction

gLPU runs on [FluffOS](https://github.com/fluffos/fluffos) and is a derivative
of the original LPUniversity mudlib and features a number of improvements and
systems. It is a work in progress and is not yet feature complete. And honestly,
it isn't really intended to be.

## Installation

You can
* download the latest release from the
  [releases page](https://github.com/gesslar/glpu/releases/latest), or
* clone it from the [gLPU repository](https://github.com/gesslar/glpu).

However you obtain it, you will need to compile the FluffOS driver and
configure your `config.mud` file as described below.

These build instructions have been tested on Ubuntu 22.04 LTS running under
WSL2 on Windows 11. However, if you feel as though your setup is significantly
different to this, you can refer to the build instructions at
[FluffOS.info](https://www.fluffos.info/build.html).

### Preparation for building the driver

* Install the dependencies required to build the FluffOS driver:

```bash
sudo apt update
sudo apt install build-essential bison libmysqlclient-dev libpcre3-dev libpq-dev \
libsqlite3-dev libssl-dev libz-dev libjemalloc-dev libicu-dev
```

* Ensure that you have pip installed. If you do not have pip installed, you can install it with the following command:

```bash
sudo apt install python3-pip
```

* Then upgrade your cmake to the latest version:

```bash
sudo pip install --upgrade cmake
```

### Building the driver

While you can certainly get your own copy of the FluffOS driver and configure
and compile it using your own ingenuity, you may find it easier to use the
bash script provided as `adm/dist/rebuild`.

This script makes a lot of assumptions and the most important one is that you
are running this script from `adm/dist`.

Running this script does a number of things, and in order they are:

1. Updates the FluffOS submodule to the latest version.
2. Copies the `local_options` file to the `fluffos` directory.
3. Runs `cmake` with a number of options.
4. Runs `make` to compile the driver.
5. When finished building, copies the binaries to the `adm/dist/bin` directory.
6. Copies the driver header files to the `include/driver` directory.
7. Resets the `fluffos` directory to the original state.
8. Updates `adm/dist/config.mud` with the absolute paths to `mudlib directory`
   and `log directory` as required.
9. Prints pretry green success messages.

## Configuration

To get things started, you really only need to configure 1 thing and another
thing is optional, but you probably should do it anyway.

Edit the `config.mud` file in the `adm/dist` directory and set the following
values:

1. Set the `external_port_1` to the port number that you want your game to
   listen on.
2. Set the `name` value to the name of your game.

As far as configuration goes, that's enough to get you going.

## Running the game

To run the game, there is a bash script provided as `adm/dist/run`. You can
run the script in one of two modes.

1. Run the game in the foreground: `./run`
2. Run the game in the background: `./run bg`
3. If you run the game in the background, or, if like me, WSL2 sometimes goes
   away but it doesn't take its running processes with it, you can use another
   argument to find all processes running the driver and kill them:
   `./run stop`. This is a third bullet point, but stopping isn't... a mode?
   I don't know. I'm not a doctor.

You can also explore using other arguments with `run`, but they won't work.

## Logging in

Use your favourite MUD client or telnet client or whatever to connect to your
game on the port that you specified in the `config.mud` file.

The first person to login to the game will become an admin. So, you know, be
careful with that. (You don't really have to be careful, it's your game now,
do what you want.)

## Contributing

If you would like to contribute to gLPU, please feel free to fork the
repository and submit a pull request. But, keep in mind that gLPU is an
opinionated fork of LPUniversity and as such, pull requests are subject to
review and may be rejected if they do not align with the goals of gLPU.

## License

I'll be frank with you. I don't really understand licensing. The original
license for LPUniversity is located in `ARCHIVE/license` and applies to all
things that originated with LPUniversity. Anything customised after that, I
don't know. I don't actually care what you do with anything that was written
by @gesslar. I, being @gesslar, make that statement.

## Come talk to us

Finally, if you love LPC and you want to talk about LPC, you are welcome to
visit us and talk about LPC on our [LPC Discord server](https://discord.gg/wzUbBgs3AQ).

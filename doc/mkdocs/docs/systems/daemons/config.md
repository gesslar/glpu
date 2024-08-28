# Configuration Daemon (CONFIG_D)

The `CONFIG` daemon is vital for managing and loading the game's configuration
settings, ensuring that your game runs with the right parameters.

## What is the Configuration Daemon?

The Configuration Daemon, defined in `adm/daemons/config.c`, is responsible for reading and holding game configuration data. It loads settings from two JSON files:
- `adm/etc/default.json`: This file contains default configuration values stored in the repository.
- `adm/etc/config.json`: This file allows for custom configuration overrides and is not stored in the repository, ensuring that your custom settings are preserved across updates.

## How it Works

When the daemon is loaded, it reads from `default.json` first to gather the baseline settings. It then reads from `config.json` to override any settings specified there. This layered approach allows you to maintain default settings while customizing specific configurations for your particular instance of the game.

## Usage

To use this daemon, simply ensure it is loaded and use the [mud_config()](/simul_efun/system.md#mud_config) simul_efun to retrieve configuration values.

### Example

```c
string autodoc_root = mud_config("AUTODOC_ROOT");
```

## Updating Configuration

The `master` command is used to manually reload the configuration settings after you have made changes to `config.json` to rehash the configuration values without restarting the game.

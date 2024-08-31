`ALARM_D` daemon is essential for scheduling and executing events at
specified times. It reads alarm configurations from text files and manages the
execution of these alarms.

## What is the Alarm Daemon?

The Alarm Daemon, defined in `adm/daemons/alarm.c`, is responsible for
scheduling and executing various types of alarms. These alarms are configured
through text files located in the `adm/etc/alarms/` directory. The daemon
supports multiple types of alarms, including one-time, hourly, daily, weekly,
and yearly alarms, as well as alarms that can be executed upon boot.

## How it Works

When the daemon is loaded, it reads the alarm configurations from the text
files in the `adm/etc/alarms/` directory. Each alarm type has a specific
pattern, and the daemon calculates the next occurrence based on these patterns.
The daemon then schedules and executes the alarms at the appropriate times.

Any number of alarm text files may exist in the `adm/etc/alarms/` directory,
and each file can contain any number of alarms. The daemon reads these files
and schedules the alarms accordingly.

This enables you to have a wide range of scheduled events in your MUD, from
daily maintenance tasks to special events that occur at specific times and
categorise them based on topic.

## Types of Alarms

The following types of alarms are supported:

- **B (Boot)**: Executes once after the game boots.
- **O (Once)**: Executes once at a specific date and time (`y-m-D@H:M`).
- **H (Hourly)**: Executes every hour at a specific minute (`M`).
- **D (Daily)**: Executes every day at a specific hour and minute (`H:M`).
- **W (Weekly)**: Executes on a specific day of the week at a specific hour and minute (`D@H:M`).
- **Y (Yearly)**: Executes once a year on a specific month, day, hour, and minute (`m-D@H:M`).

`ONCE`-type alarms are not parsed from the configuration files. Instead, you
should use the [add_once](#adding-a-one-time-alarm) function to add them
programmatically.

## Usage

To use this daemon, ensure it is loaded and that your alarm configuration files
are placed in the `adm/etc/alarms/` directory. You can manually reload the
alarms by running the `master` admin command.

## Alarm format

The format for an alarm in an alarm text file is `TYPE MASTER PATTERN /FILE FUNCTION [ARGUMENTS]`.

The arguments are as follows:

- **TYPE**: The type of alarm (B, O, H, D, W, Y).
- **MASTER**: Whether the alarm requires master permissions (true/false).
- **PATTERN**: The time pattern for the alarmm corresponding to the alarm type.
- **FILE**: The file containing the function to be called, must be absolute.
- **FUNCTION**: The function to be called.
- **ARGUMENTS**: Optional arguments to be passed to the function.

!!! example "Example alarm"

    ```
    Y 12-25@10:00 false /adm/daemons/holiday broadcast_message "Merry Christmas!"
    H 30 false /adm/daemons/config rehash_config
    ```


An example alarm confirmation text file is provided in `adm/etc/alarms/alarm.txt.example`.

### Adding Alarms

You can add alarms programmatically, however, you should know that alarms added
in this way are not persisted loads of the alarm daemon, which includes
reboots.

### Adding a one-time alarm

To add a one-time alarm, use the `add_once` function with the following arguments:

- **MASTER**: Either `true` or `false` depending on whether the alarm requires master permissions.
- **PATTERN**: The time pattern for the alarm (`y-m-D@H:M`).
- **FILE**: The file containing the function to be called, must be absolute.
- **FUNCTION**: The function to be called.
- **ARGUMENTS**: Optional arguments to be passed to the function.

!!! example "Adding a one-time alarm"

    ``` c
    ALARM_D->add_once("false", "12-25@10:00", "/adm/daemons/holiday", "broadcast_message", "Merry Christmas!") ;
    ```

The alarm daemon will return an identifier which you can than use to remove the
alarm, if necessary.

## Updating Alarms

To update the alarms after modifying the configuration files, use the `master`
command. This will reload the alarm configurations and update the scheduled
events.

Note that alarms added programmatically will be wiped out on a reload.

## Conclusion

The Alarm Daemon is a powerful tool for scheduling and managing events in your
MUD. By leveraging the various types of alarms and the flexible configuration
options, you can automate a wide range of tasks and ensure that they are
executed at the right times.

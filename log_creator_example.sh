#!/bin/bash

## Run this script on startup

full_path_to_log=/path/to/log/log.txt

current_date=`date +"%d-%m-%Y %T"`;
echo $current_date + " User login - Welcome back :)" >> $full_path_to_log # Print date & time to log (separate with `+` sign).

dbus-monitor --session "type='signal',interface='com.ubuntu.Upstart0_6'" | \
(
    while read X; do
        current_date=`date +"%d-%m-%Y %T"`; # Get current date and time
        if [[ "$X" =~ desktop-lock ]]; then
            echo $current_date + " User logout - See you later :)" >> $full_path_to_log # Print date & time to log (separate with `+` sign).
        elif [[ "$X" =~ desktop-unlock ]]; then
            echo $current_date + " User login - Welcome back :)" >> $full_path_to_log # Print date & time to log (separate with `+` sign).
        fi
    done
)
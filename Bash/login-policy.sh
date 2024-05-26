#!/bin/bash
# Force the script to be run as Super User or user with sudo permissions
if [[ $EUID -ne 0 ]]; then
        scriptname=$(basename "$0")
        echo "This script ($scriptname) must be run as root or with sudo permissions"
        exit 1
fi

# Ensure there are exactly three parameters
if [[ $# -ne 3 ]]; then
        echo "Usage: $0 <username> <start_time> <end_time>"
        echo "Example: $0 username \"07:30\" \"19:30\""
        exit 1
fi

# Specify the username
username=$1

# Check if the username is valid
if ! id "$username" >/dev/null 2>&1; then
        echo "User '$username' does not exist"
        exit 1
fi

# Validate the time format for the start and end times
if ! [[ $2 =~ ^([01][0-9]|2[0-3]):[0-5][0-9]$ ]] || ! [[ $3 =~ ^([01][0-9]|2[0-3]):[0-5][0-9]$ ]]; then
        echo "Invalid time format. Please use the format \"HH:MM\"."
        exit 1
fi

echo "Applying login policy to $username"

# Get the current hour and minute
current_time=$(date +%s)

# Define the start and end times in Unix timestamp format
start_time=$(date -d"$2" +%s)
end_time=$(date -d"$3" +%s)

# Check if the user account is locked and store the result as true or false
is_locked=$(passwd -S $username | grep -q "L" && echo true || echo false)

# Check if time is in range
if (( current_time >= start_time && current_time < end_time )); then
        if $is_locked; then
                /usr/sbin/usermod -U $username
                echo "User account unlocked"
        else
                echo "User account already unlocked"
        fi
else
        if $is_locked; then
                echo "User account already locked"
        else
                /usr/sbin/usermod -L $username
                if who | grep -q "^$username "; then
                        pkill -KILL -u $username
                        echo "User account locked and all processes killed"
                fi
        fi
fi
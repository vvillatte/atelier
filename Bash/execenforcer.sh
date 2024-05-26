#!/bin/bash
# Force the script to be run as Super User or user with sudo permissions
if [[ $EUID -ne 0 ]]; then
        echo "This script must be run as root or with sudo permissions"
        exit 1
fi

# Ensure there are exactly three parameters
if [[ $# -ne 5 ]]; then
        echo "Usage: $0 <program_path> <start_time> <end_time> <max_duration>"
        echo "Example: $0 /usr/bin/discord \"17:00\" \"19:00\" \"120\""
        exit 1
fi

# Specify the program path
username=$1
program=$2

# Validate the time format for the start and end times
if ! [[ $3 =~ ^([01][0-9]|2[0-3]):[0-5][0-9]$ ]] || ! [[ $4 =~ ^([01][0-9]|2[0-3]):[0-5][0-9]$ ]]; then
        echo "Invalid time format. Please use the format \"HH:MM\"."
        exit 1
fi

# Check if the username is valid
if ! id "$username" >/dev/null 2>&1; then
        echo "User '$username' does not exist"
        exit 1
fi

# Check if the program exists
if [[ ! -e "$program" ]]; then
        echo "The program does not exist."
        exit 1
fi

target="$program"
application=$(basename "$program")

# Check for Symlink
if [[ -L "$program" ]]; then
        target=$(readlink -f "$program")
fi

echo "Applying execution policy to $application for $username"

# Get the current hour and minute
current_time=$(date +%s)

# Define the start and end times in Unix timestamp format
start_time=$(date -d"$3" +%s)
end_time=$(date -d"$4" +%s)

# Check if the program is executable and store the result as true or false
is_executable=$( [[ -x "$program" ]] && echo true || echo false )

# Check if the program is running and store the result as true or false
is_running=$(pgrep -fu "$username" "$application" && echo true || echo false)

# Kill and disable the application if it has run more than the allowed time
if (( total_running_time > max_duration )); then
        if $is_running; then
                /usr/bin/pkill -fu "$username" "$application"
                echo "$(date): Program was running more than the allowed duration and has been killed" >> "$log_file"
        fi
        chmod -x $program
        echo "Program has been disabled"
        exit 1
elif (( current_time >= start_time && current_time < end_time )); then
        if $is_executable; then
                echo "Program already executable"
        else
                chmod +x $program
                echo "Program execution enabled"
        fi
else
        if $is_executable; then
                chmod -x $program
                echo "Program execution disabled"
        else
                echo "Program already non-executable"
        fi
        if $is_running; then
                /usr/bin/pkill -fu "$username" "$application"
                echo "Program was running outside the allowed time range and has been killed"
        else
                echo "Program is not running"
        fi
fi
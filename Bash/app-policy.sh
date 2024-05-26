#!/bin/bash
# Force the script to be run as Super User or user with sudo permissions
if [[ $EUID -ne 0 ]]; then
        echo "This script must be run as root or with sudo permissions"
        exit 1
fi

# Ensure there are exactly three parameters
if [[ $# -ne 5 ]]; then
        echo "Usage: $0 <app_path> <start_time> <end_time> <limit>"
        echo "Example: $0 /usr/bin/discord \"17:00\" \"19:00\" \"120\""
        exit 1
fi

username=$1
app_path=$2

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

# Check if the Application exists
if [[ ! -e "$app_path" ]]; then
        echo "The Application $app_path does not exist."
        exit 1
fi

target="$app_path"
app_name=$(basename "$app_path")
log_file=$"/var/log/$username-$app_name.log"

# Check for Symlink
if [[ -L "$app_path" ]]; then
        target=$(readlink -f "$app_path")
fi

echo "Applying execution policy to $app_name for $username"

# Get the current hour and minute
current_time=$(date +%s)

# Define the start and end times in Unix timestamp format
start_time=$(date -d"$3" +%s)
end_time=$(date -d"$4" +%s)

# Check if the Application is executable and store the result as true or false
is_executable=$( [[ -x "$app_path" ]] && echo true || echo false )

# Check if the Application is running and store the result as true or false
is_running=$(pgrep -fu "$username" "$app_name" > /dev/null && echo true || echo false)

# Read the Total Daily Runtime from application log
if [[ -e "$log_file" ]]; then
        total_runtime=$(grep "Total Daily Runtime:" $log_file | cut -d ' ' -f4)
        echo "Application $app_name has been used for $total_runtime minutes"
        # Kill the application if it has run more than the allowed time
        if (( total_runtime >= limit )) && [[ $is_runnning == "true" ]]; then
                /usr/bin/pkill -fu "$username" "$app_name"
                echo "The application was running more than the allowed duration and has been killed"
        fi
else
        echo "Log file $log_file not found"
fi

# Set the application executable flag as per policy
if (( current_time >= start_time && current_time < end_time )); then
        if [[ $is_executable == "true" ]]; then
                echo "Application already executable"
        else
                chmod +x $app_path
                echo "Application execution enabled"
        fi
else
        if [[ $is_executable == "true" ]]; then
                chmod -x $app_path
                echo "Application execution disabled"
        else
                echo "Application already non-executable"
        fi
        if [[ $is_running == "true" ]]; then
                /usr/bin/pkill -fu "$username" "$app_name"
                echo "Application $app_name was running outside the allowed time range and has been killed"
        else
                echo "Application is not running"
        fi
fi
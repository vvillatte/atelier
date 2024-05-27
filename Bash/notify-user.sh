#!/bin/bash

# Check if jq is installed
if ! command -v jq &> /dev/null; then
  echo "jq could not be found. Please install it and try again."
  exit 1
fi

# Define the location of the settings file
settings_file="/usr/local/sbin/parental-controls/pc-settings.json"

# Read the settings from the pc-settings.json file
settings=$(cat $settings_file)
#username=$(echo $settings | jq -r '.user.username')
username="vvillatte"
app_path=$(echo $settings | jq -r '.user.application.path')
#app_name=$(basename $app_path)
app_name="spotify"

# Define the location of the log file
log_dir="/var/log/parental-controls/"
log_file="${log_dir}${username}-${app_name}.log"

# Check if the username in the settings file is the same as the user running this script
if [ "$username" != "$(whoami)" ]; then
  echo "The username in the settings file does not match the user running this script."
  exit 1
fi

# Get the current day of the week
day=$(date +%A | tr '[:upper:]' '[:lower:]')

if pgrep -u $username -x $app_name > /dev/null; then
        # Get the application limit for the current day
        app_limit=$(echo $settings | jq -r ".user.application.$day.limit")
        echo "(DEBUG): Application limit: $app_limit"

        # Read the log file
        log=$(cat $log_file)

        # Get the total daily runtime
        total_runtime=$(echo $log | grep -oP 'Total Daily Runtime: \K\d+')
        echo "(DEBUG): Total runtime: $total_runtime"

        # Calculate the remaining time
        remaining_time=$((app_limit - total_runtime))
        echo "(DEBUG): Remaining time: $remaining_time"

        # Check if the application is getting close to being killed and disabled
        if ((remaining_time <= 1)); then
          notify-send "Warning: The application $app_name will close in 1 minute."
        elif ((remaining_time <= 5)); then
          notify-send "Warning: The application $app_name will close in 5 minutes."
        elif ((remaining_time <= 10)); then
          notify-send "Warning: The application $app_name will close in 10 minutes."
        fi
else
        echo "The application $app_name is not running for the user $username"
fi

# Do the same for the login end time
login_end=$(echo $settings | jq -r ".user.login.$day.end")
login_end_seconds=$(date -d"$login_end" +%s)
current_time_seconds=$(date +%s)

# Calculate the remaining login time
remaining_login_time=$((login_end_seconds - current_time_seconds))

# Check if the login time is getting close to ending
if ((remaining_login_time <= 60)); then
  notify-send "Warning: The login time will end in 1 minute."
elif ((remaining_login_time <= 300)); then
  notify-send "Warning: The login time will end in 5 minutes."
elif ((remaining_login_time <= 600)); then
  notify-send "Warning: The login time will end in 10 minutes."
fi
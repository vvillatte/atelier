#!/bin/bash
export SHELL=/bin/sh
export PATH=$PATH:/usr/local/sbin

# Path to the JSON file
json_file="/usr/local/sbin/parentalsettings.json"

# Check if jq is installed
if ! command -v jq &> /dev/null
then
    echo "jq could not be found, please install it first."
    exit
fi

# Read the JSON file
username=$(jq -r '.user.username' "$json_file")

# Get the current day of the week
current_day=$(date +%A | tr '[:upper:]' '[:lower:]')

# Read the login and application usage schedule for the current day
login_start=$(jq -r ".user.login.$current_day.start" "$json_file")
login_end=$(jq -r ".user.login.$current_day.end" "$json_file")
app_usage_start=$(jq -r ".user.application.$current_day.start" "$json_file")
app_usage_end=$(jq -r ".user.application.$current_day.end" "$json_file")
app_usage_max_duration=$(jq -r ".user.application.$current_day.max_duration" "$json_file")

# Print the scheduling rules for the current day
echo "$current_day Login Start Time: $login_start"
echo "$current_day Login End Time: $login_end"
echo "$current_day Application Usage Start Time: $app_usage_start"
echo "$current_day Application Usage End Time: $app_usage_end"
echo "$current_day Application Usage Duration: $app_usage_max_duration"

program=$(jq -r '.user.application.path' "$json_file")
echo "Application Path: $program"

# Apply program and login rules
execenforcer.sh "$username" "$program" "$app_usage_start" "$app_usage_end" "$app_usage_max_duration"
loginenforcer.sh "$username" "$login_start" "$login_end"

current_time=$(date +%s)
midnight=$(date -d"00:00" +%s)
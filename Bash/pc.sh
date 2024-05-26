#!/bin/bash
# Get the path of the current script
dir_path="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"

# Define Ppath to the JSON configuration file
config_file="$dir_path/pc-settings.json"

# Check if jq is installed
if ! command -v jq &> /dev/null
then
    echo "jq could not be found, please install it first."
    exit
fi

# Check that the configuration file exists
if [[ ! -e "$config_file" ]]; then
        echo "$(date) ==> Configuration file $config_file not found"
        exit
fi

# Get the current day of the week
current_day=$(date +%A | tr '[:upper:]' '[:lower:]')

# Read the login and application usage schedule for the current day
username=$(jq -r '.user.username' "$config_file")
login_start=$(jq -r ".user.login.$current_day.start" "$config_file")
login_end=$(jq -r ".user.login.$current_day.end" "$config_file")
app_path=$(jq -r '.user.application.path' "$config_file")
app_usage_start=$(jq -r ".user.application.$current_day.start" "$config_file")
app_usage_end=$(jq -r ".user.application.$current_day.end" "$config_file")
app_usage_limit=$(jq -r ".user.application.$current_day.limit" "$config_file")

# Print the scheduling rules for the current day
echo "$current_day Login Start Time: $login_start"
echo "$current_day Login End Time: $login_end"
echo "Application Path: $app_path"
echo "$current_day Application Usage Start Time: $app_usage_start"
echo "$current_day Application Usage End Time: $app_usage_end"
echo "$current_day Application Usage Limit: $app_usage_limit"

# Apply program and login rules
"$dir_path/app-policy.sh" "$username" "$app_path" "$app_usage_start" "$app_usage_end" "$app_usage_limit"
"$dir_path/login-policy.sh" "$username" "$login_start" "$login_end"
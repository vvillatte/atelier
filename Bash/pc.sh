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

# Read the entire JSON file into a variable
config=$(cat "$config_file")

# Read the login and application usage schedule for the current day
username=$(jq -r '.user.login.name' <<< "$config")
login_start=$(jq -r ".user.login.schedule.$current_day.start" <<< "$config")
login_end=$(jq -r ".user.login.schedule.$current_day.end" <<< "$config")
app_name=$(jq -r '.user.application.name' <<< "$config")
app_path=$(jq -r '.user.application.path' <<< "$config")
app_usage_start=$(jq -r ".user.application.schedule.$current_day.start" <<< "$config")
app_usage_end=$(jq -r ".user.application.schedule.$current_day.end" <<< "$config")
app_usage_limit=$(jq -r ".user.application.schedule.$current_day.limit" <<< "$config")
site_name=$(jq -r '.user.website.name' <<< "$config")
site_url=$(jq -r '.user.website.url' <<< "$config")
site_usage_start=$(jq -r ".user.website.schedule.$current_day.start" <<< "$config")
site_usage_end=$(jq -r ".user.website.schedule.$current_day.end" <<< "$config")

# Print the scheduling rules for the current day
echo "User name: $username"
echo "$current_day Login Start Time: $login_start"
echo "$current_day Login End Time: $login_end"
echo "Application anme: $app_name"
echo "Application Path: $app_path"
echo "$current_day Application Usage Start Time: $app_usage_start"
echo "$current_day Application Usage End Time: $app_usage_end"
echo "$current_day Application Usage Limit: $app_usage_limit"
echo "Website: $site_name"
echo "Website url: $site_url"
echo "$current_day Website Usage Start Time: $app_usage_start"
echo "$current_day Website Usage End Time: $app_usage_end"
echo "=== *** ==="

# Apply program and login rules
"$dir_path/app-policy.sh" "$username" "$app_path" "$app_usage_start" "$app_usage_end" "$app_usage_limit"
echo "=== *** ==="
"$dir_path/web-policy.sh" "$site_url" "$site_usage_start" "$site_usage_end"
sudo /usr/sbin/squid -k reconfigure
echo "=== *** ==="
"$dir_path/login-policy.sh" "$username" "$login_start" "$login_end"
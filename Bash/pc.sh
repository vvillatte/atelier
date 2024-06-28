#!/bin/bash
# Get the path of the current script
dir_path="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"

# Define path to the JSON configuration file
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
echo "Date: $current_day"

# Read the entire JSON file into a variable
config=$(cat "$config_file")

# Step 1: Retrieve user name
username=$(jq -r '.user.login.name' <<< "$config")
echo "User name: $username"

# Step 2: Apply applications execution policy
num_apps=$(jq '.user.applications | length' <<< "$config")

echo ""
echo "################################"
echo "# Applications excution policy #"
echo "################################"
# Loop over each application
for (( i=0; i<$num_apps; i++ ))
do
        app_name=$(jq -r ".user.applications[$i].name" <<< "$config")
        app_path=$(jq -r ".user.applications[$i].path" <<< "$config")
        app_usage_start=$(jq -r ".user.applications[$i].schedule.$current_day.start" <<< "$config")
        app_usage_end=$(jq -r ".user.applications[$i].schedule.$current_day.end" <<< "$config")
        app_usage_limit=$(jq -r ".user.applications[$i].schedule.$current_day.limit" <<< "$config")

        echo ""
        echo "Application name: $app_name"
        echo "Application Path: $app_path"
        echo "$current_day Application Usage Start Time: $app_usage_start"
        echo "$current_day Application Usage End Time: $app_usage_end"
        echo "$current_day Application Usage Limit: $app_usage_limit"

        "$dir_path/app-policy.sh" "$username" "$app_path" "$app_usage_start" "$app_usage_end" "$app_usage_limit"
done

echo ""
echo "##########################"
echo "# Websites access policy #"
echo "##########################"
# Step 3: Apply web access policy
num_sites=$(jq '.user.websites | length' <<< "$config")

for (( i=0; i<$num_sites; i++ ))
do
        site_name=$(jq -r ".user.websites[$i].name" <<< "$config")
        site_url=$(jq -r ".user.websites[$i].url" <<< "$config")
        site_usage_start=$(jq -r ".user.websites[$i].schedule.$current_day.start" <<< "$config")
        site_usage_end=$(jq -r ".user.websites[$i].schedule.$current_day.end" <<< "$config")

        echo ""
        echo "Website: $site_name"
        echo "Website url: $site_url"
        echo "$current_day Website Usage Start Time: $site_usage_start"
        echo "$current_day Website Usage End Time: $site_usage_end"

        "$dir_path/web-policy.sh" "$site_url" "$site_usage_start" "$site_usage_end"
done
sudo /usr/sbin/squid -k reconfigure

echo ""
echo "################"
echo "# Login policy #"
echo "################"
# Step 4: Apply login policy
login_start=$(jq -r ".user.login.schedule.$current_day.start" <<< "$config")
login_end=$(jq -r ".user.login.schedule.$current_day.end" <<< "$config")

echo ""
echo "$current_day Login Start Time: $login_start"
echo "$current_day Login End Time: $login_end"

"$dir_path/login-policy.sh" "$username" "$login_start" "$login_end"
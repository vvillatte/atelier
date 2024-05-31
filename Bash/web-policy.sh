#!/bin/bash

# Force the script to be run as Super User or user with sudo permissions
if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root or with sudo permissions"
    exit 1
fi

# Ensure there are exactly 3 parameters
if [[ $# -ne 3 ]]; then
    echo "Usage: $0 <url> <start_time> <end_time>"
    echo "Example: $0 .domain.tld \"17:00\" \"19:00\""
    exit 1
fi

# Validate the time format for the start and end times
if ! [[ $2 =~ ^([01][0-9]|2[0-3]):[0-5][0-9]$ ]] || ! [[ $3 =~ ^([01][0-9]|2[0-3]):[0-5][0-9]$ ]]; then
    echo "Invalid time format. Please use the format \"HH:MM\"."
    exit 1
fi

url=$1
acl_path="/etc/squid/domains_whitelist.acl"
regex='^(#?\.[a-zA-Z0-9.-]+\.[a-zA-Z]{2,6})$'

# Check that the url is well formed
if [[ ! $url =~ $regex ]]; then
    echo "The URL is not well-formed"
    exit 1
fi

# Check if the url exists in domain_whitelist.acl
if ! grep -Eq "$url" "$acl_path"; then
    echo "Adding domain $url to $acl_path"
    echo "$url" >> "$acl_path"
fi

echo "Applying web policy to $url"

# Get the current hour and minute
current_time=$(date +%s)

# Define the start and end times in Unix timestamp format
start_time=$(date -d"$2" +%s)
end_time=$(date -d"$3" +%s)

is_blocked=$(grep -Eq "^#.*$url" "$acl_path" && echo true || echo false)

# Check if time is in range and if the url is commented out
if (( current_time >= start_time && current_time < end_time )); then
    if [[ $is_blocked == "true" ]]; then
        sed -i "/^#.*$url/s/^#//" "$acl_path"
        echo "Domain $url is accessible"
    else
        echo "Domain $url already accessible"
    fi
else
    if [[ $is_blocked == "true" ]]; then
        echo "Domain $url already blocked"
    else
        sed -i "/^$url/s/^/#/" "$acl_path"
        echo "Domain $url blocked"
    fi
fi

# Update squid configuration without restarting the server
#/usr/sbin/squid -k reconfigure
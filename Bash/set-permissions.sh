#!/bin/bash

# Function to display help message
function display_help {
    echo "Usage: $0 -u username -g groupname -p permissions [-r] /path/to/start"
    echo ""
    echo "Options:"
    echo "  -u  Username for chown"
    echo "  -g  Groupname for chown"
    echo "  -p  Permissions for chmod"
    echo "  -r  Perform the operation recursively"
    echo "  -h  Display this help message"
}

# Parse command line options
recursive=false
while getopts "u:g:p:rh" opt; do
    case $opt in
        u) username="$OPTARG" ;;
        g) groupname="$OPTARG" ;;
        p) permissions="$OPTARG" ;;
        r) recursive=true ;;
        h) display_help; exit 0 ;;
        ?) display_help; exit 1 ;;
    esac
done

# Shift off the options and optional --
shift $((OPTIND-1))

# Check if the start directory is provided
if [ -z "$1" ]; then
    echo "Error: The start directory is missing."
    display_help
    exit 1
fi

START_DIR="$1"

# Check if username, groupname, and permissions are provided
if [ -z "$username" ] || [ -z "$groupname" ] || [ -z "$permissions" ]; then
    echo "Error: Username, groupname, or permissions are missing."
    display_help
    exit 1
fi

# Function to update ownership and permissions
update_ownership_permissions() {
    if $recursive; then
        find "$1" -not -path "*/@eaDir/*" -not -path "*/#recycle/*" -exec chown "$username:$groupname" {} \;
        find "$1" -not -path "*/@eaDir/*" -not -path "*/#recycle/*" -exec chmod "$permissions" {} \;
    else
        chown "$username:$groupname" "$1"
        chmod "$permissions" "$1"
    fi
}

# Perform the update
update_ownership_permissions "$START_DIR"

echo "Ownership and permissions updated successfully, except for @eaDir and #recycle folders."
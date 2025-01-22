#!/bin/bash

# Function to display help message
function display_help {
    echo "Usage: $0 -u username -g groupname -p permissions [-rcv] /path/to/start"
    echo ""
    echo "Options:"
    echo "  -u  Username for chown"
    echo "  -g  Groupname for chown"
    echo "  -p  Permissions for chmod"
    echo "  -r  Perform the operation recursively"
    echo "  -c  Clear Access Control Lists (ACLs)"
    echo "  -v  Verbose output"
    echo "  -h  Display this help message"
    echo ""
    echo "Description:"
    echo "  This script updates the ownership and permissions of files and directories."
    echo "  - It takes a starting directory as input and processes all files and directories within it."
    echo "  - You can specify the username and groupname for ownership, as well as the permissions."
    echo "  - The script can operate recursively if the -r flag is provided."
    echo "  - It can also clear any existing Access Control Lists (ACLs) if the -c flag is provided."
    echo "  - The script provides verbose output if the -v flag is provided."
    echo "  - Folders '@eaDir' and '#recycle' are excluded from the operation."
}

# Parse command line options
recursive=false
clear_acls=false
verbose=false
while getopts "u:g:p:rcvh" opt; do
    case $opt in
        u) username="$OPTARG" ;;
        g) groupname="$OPTARG" ;;
        p) permissions="$OPTARG" ;;
        r) recursive=true ;;
        c) clear_acls=true ;;
        v) verbose=true ;;
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
        find "$1" -not -path "*/@eaDir/*" -not -path "*/#recycle/*" | while read -r file; do
            if $clear_acls; then
                setfacl -b "$file"
                $verbose && echo "Cleared ACLs for $file"
            fi
            chown "$username:$groupname" "$file"
            $verbose && echo "Updated ownership to $username:$groupname for $file"
            chmod "$permissions" "$file"
            $verbose && echo "Updated permissions to $permissions for $file"
        done
    else
        if $clear_acls; then
            setfacl -b "$1"
            $verbose && echo "Cleared ACLs for $1"
        fi
        chown "$username:$groupname" "$1"
        $verbose && echo "Updated ownership to $username:$groupname for $1"
        chmod "$permissions" "$1"
        $verbose && echo "Updated permissions to $permissions for $1"
    fi
}

# Perform the update
update_ownership_permissions "$START_DIR"

echo "Ownership and permissions updated successfully, except for @eaDir and #recycle folders."
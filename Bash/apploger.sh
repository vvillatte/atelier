#!/bin/bash

# Check if the script is run by super user or user with sudo privileges
if [ "$EUID" -ne 0 ]; then
        echo "Please run as root or use sudo."
        exit 1
fi

# Check if username and process name are provided
if [ -z "$1" ] || [ -z "$2" ]; then
        echo "Usage: $0 <username> and <process name>."
        echo "Example: $0 aeinstein discord"
        exit 1
fi

# Check if the user exists
if ! id -u $1 > /dev/null 2>&1; then
        echo "User $1 does not exist."
        exit 1
fi

# Get the username and process name
username=$1
process_name=$2

# Define the log file
log_file="/var/log/$username-$process_name.log"

# Create the log file if it does not exist
touch $log_file

# Get the current date
current_date=$(date +%Y-%m-%d)

# Check if the log file is empty or does not contain today's date
if [ ! -s $log_file ] || ! grep -q "$current_date" $log_file; then
        echo "Date: $current_date" > $log_file
        echo "First Use Start Time: N/A" >> $log_file
        echo "Current Session Start Time: N/A" >> $log_file
        echo "Current Session Runtime: N/A" >> $log_file
        echo "Previous Sessions Runtime: 0 minutes" >> $log_file
        echo "Total Daily Runtime: 0 minutes" >> $log_file
fi

# Get the process ID
pid=$(pgrep -fu $username $process_name | head -n 1)

# Check if process is running
if [ -z "$pid" ]; then
        # If the process is not running, update the total usage time in the log file
        if grep -q "Current Session Start Time: N/A" $log_file; then
                echo "Process $process_name is not running for user $username."
        else
                session_start_time=$(grep "Current Session Start Time:" $log_file | cut -d' ' -f5-)
                session_start_time_seconds=$(date -d"$session_start_time" +%s)
                current_time_seconds=$(date +%s)
                running_time=$(( (current_time_seconds - session_start_time_seconds) / 60 ))
                previous_sessions_runtime=$(grep "Previous Sessions Runtime:" $log_file | cut -d' ' -f4)
                previous_sessions_runtime=$((previous_sessions_runtime + running_time))
                total_daily_runtime=$((previous_sessions_runtime))
                sed -i "s|Current Session Runtime: .*|Current Session Runtime: N/A|" $log_file
                sed -i "s|Previous Sessions Runtime: .*|Previous Sessions Runtime: $previous_sessions_runtime minutes|" $log_file
                sed -i "s|Total Daily Runtime: .*|Total Daily Runtime: $total_daily_runtime minutes|" $log_file
                sed -i "s|Current Session Start Time: .*|Current Session Start Time: N/A|" $log_file
        fi
else
        session_start_time=$(ps -eo pid,lstart | grep "^ *$pid" | head -n 1 | awk '{$1=""; print $0}')
        session_start_time=$(date -d"$session_start_time" +"%Y-%m-%d %H:%M:%S")
        # If the process is running and the start time in the log file is not "N/A", update it
        if grep -q "Current Session Start Time: N/A" $log_file; then
                sed -i "s|Current Session Start Time: N/A|Current Session Start Time: $session_start_time|" $log_file
                if grep -q "First Use Start Time: N/A" $log_file; then
                        sed -i "s|First Use Start Time: N/A|First Use Start Time: $session_start_time|" $log_file
                fi
        fi
        session_start_time_seconds=$(date -d"$session_start_time" +%s)
        current_time_seconds=$(date +%s)
        running_time=$(( (current_time_seconds - session_start_time_seconds) / 60 ))
        sed -i "s|Current Session Runtime: .*|Current Session Runtime: $running_time minutes|" $log_file
        previous_sessions_runtime=$(grep "Previous Sessions Runtime:" $log_file | cut -d' ' -f4)
        total_daily_runtime=$((previous_sessions_runtime + running_time))
        sed -i "s|Total Daily Runtime: .*|Total Daily Runtime: $total_daily_runtime minutes|" $log_file
fi

# Display the content of the log file
cat $log_file
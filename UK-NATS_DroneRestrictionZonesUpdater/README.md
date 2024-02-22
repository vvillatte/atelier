```markdown
# UK-NATS Drone Restriction Zones Updater

This project contains Python scripts to download XML and KMZ files from a specific webpage and store them in a 'data' folder. The scripts are designed to be run as standalone programs or imported as modules in other scripts.

## Files

- `downloadXML.py`: This script downloads an XML file from a specific webpage and stores it in a 'data' folder.
- `downloadKMZ.py`: This script downloads a KMZ file from a specific webpage, stores it in a 'data' folder, and extracts its contents.
- `UK-NATS-DRZ_Updater.py`: This is the main script that calls the `downloadXML` and `downloadKMZ` functions and prints the file paths.

## Usage

You can run the scripts individually using Python 3:

```bash
python3 downloadXML.py
python3 downloadKMZ.py
python3 UK-NATS-DRZ_Updater.py
```

Or you can import the `downloadXML` and `downloadKMZ` functions in another script and call them:

```python
import downloadXML
import downloadKMZ

# Call the downloadXML function and get the file path
XML_file_path = downloadXML.downloadXML()

# Call the downloadKMZ function and get the file path
kmz_file_path = downloadKMZ.downloadKMZ()
```

## Author

Vincent Villatte

## Version

1.0

## Date

22/02/2024

## Scheduling the update
You can then schedule this script to run every 28 days (AIRAC cycle) using a task scheduler.
The exact command will depend on your system configuration and where the Python script is saved, but it will look something like this:

### Linux
Running a cron job on Linux or MacOS

```bash
0 0 */28 * * /usr/bin/python3 /path/to/your/script.py
```

This will run the script at midnight every 28 days. Please replace `/usr/bin/python3` with the path to your Python interpreter, and `/path/to/your/script.py` with the path to your Python script. You can edit your crontab file by running `crontab -e` in your terminal. Please note that cron uses a different environment than your interactive shell, so you might need to specify full paths to your Python interpreter and script. Also, the user that the cron job runs as will need to have permission to execute the Python script and write to the directory where the files are downloaded.

### Windows
On Windows, you can use the Task Scheduler to run a script at regular intervals. Here's a step-by-step guide on how to do it:

1. Open the Task Scheduler. You can do this by searching for "Task Scheduler" in the Start menu.
2. In the Task Scheduler, click on "Create Basic Task..." in the Actions panel on the right.
3. In the "Create Basic Task Wizard" window that appears, enter a name and a description for the task, then click "Next".
4. On the "Task Trigger" screen, select "Daily", then click "Next".
5. On the "Daily" screen, set the "Recur every" field to 28 days, then click "Next".
6. On the "Action" screen, select "Start a program", then click "Next".
7. On the "Start a Program" screen, click "Browse..." and navigate to the Python executable file (`python.exe`). This is usually located in the `Scripts` folder of your Python installation directory. In the "Add arguments (optional)" field, enter the path to your Python script. Then click "Next".
8. On the "Finish" screen, review your settings, then click "Finish".

Your Python script is now scheduled to run every 28 days. Please note that the user account you're logged in with must have the necessary permissions to run the Python script and the Task Scheduler.
```
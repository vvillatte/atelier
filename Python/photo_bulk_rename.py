"""
This script renames all files in a given directory from the pattern 'YYYYMMDD_HHMMSS.ext' 
to the pattern 'YYYY-MM-DDTHH_MM_SS-000001.ext', where 'ext' is any file extension. 
If the directory is not provided as a command line argument, it prompts the user to enter it.

Usage: python rename_files.py <directory>
"""

import os
import glob
import sys
import re
from tqdm import tqdm

def rename_files(directory):
    """
    This function renames all files in a given directory from the pattern 'YYYYMMDD_HHMMSS.ext' 
    to the pattern 'YYYY-MM-DDTHH_MM_SS-000001.ext', where 'ext' is any file extension.

    Args:
        directory (str): The directory containing the files to be renamed.

    """
    # Check if the directory exists
    if not os.path.isdir(directory):
        print(f"Error: The directory {directory} does not exist.")
        return

    # Get all files in the directory
    files = glob.glob(os.path.join(directory, '*'))

    # Define the pattern that the files should match
    pattern = re.compile(r'\d{8}_\d{6}(\(\d+\))?\..*')

    # Filter the files that match the pattern
    files_to_rename = [file for file in files if pattern.match(os.path.basename(file))]

    # Create a progress bar
    pbar = tqdm(total=len(files_to_rename), ncols=70)

    # Renamed file counter
    file_count = 0

    for file in files_to_rename:
        # Extract the base name of the file and the file extension
        base_name, ext = os.path.splitext(os.path.basename(file))
        # Remove the tag from the base name
        base_name = re.sub(r'\(\d+\)', '', base_name)
        # Extract the date and time from the base name
        date, time = base_name.split('_')
        # Format the date and time
        formatted_date = f"{date[:4]}-{date[4:6]}-{date[6:8]}"
        formatted_time = f"{time[:2]}_{time[2:4]}_{time[4:6]}"
        # Create the new file name
        file_count += 1
        new_name = f"{formatted_date}T{formatted_time}-{file_count:03}{ext}"
        # Create the new file path
        new_path = os.path.join(directory, new_name)
        # Rename the file
        os.rename(file, new_path)
        # Update the progress bar
        pbar.update(1)

    # Close the progress bar
    pbar.close()

def main():
    """
    The main function that gets called when the script is run from the command line.
    """
    # Check if the directory was passed as a command line argument
    if len(sys.argv) != 2:
        directory = input("Please enter the directory where the photos are located: ")
    else:
        directory = sys.argv[1]

    # Call the rename_files function with the directory
    rename_files(directory)

if __name__ == "__main__":
    main()

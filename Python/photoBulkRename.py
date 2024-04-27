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
    # Get all files in the directory
    files = glob.glob(os.path.join(directory, '*'))

    # Define the pattern that the files should match
    pattern = re.compile(r'\d{8}_\d{6}(\(\d+\))?\..*')

    # Create a progress bar
    pbar = tqdm(total=len(files), ncols=70)

    # Renamed file counter
    file_count = 0

    for i, file in enumerate(files, start=1):
        # Extract the base name of the file and the file extension
        base_name, ext = os.path.splitext(os.path.basename(file))
        # Check if the file matches the pattern
        if not pattern.match(base_name + ext):
            continue
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
        # print(file, new_path)
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
        print("Usage: python rename_files.py <directory>")
        sys.exit(1)

    # Call the rename_files function with the directory passed as a command line argument
    rename_files(sys.argv[1])

if __name__ == "__main__":
    main()

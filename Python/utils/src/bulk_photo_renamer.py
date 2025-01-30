import os
import sys
import re
from tqdm import tqdm
from colorama import Fore, Style, init

# Initialize colorama
init(autoreset=True)

def rename_photos(directory, test_mode=False, verbose=False):
    """
    This function renames all files in a given directory from the pattern 'YYYYMMDD_HHMMSS.ext'
    to the pattern 'YYYY-MM-DDTHH_MM_SS-000001.ext', where 'ext' is any file extension.

    Args:
        directory (str): The directory containing the files to be renamed.
        test_mode (bool): If True, print the new names without renaming the files.
        verbose (bool): If True, print detailed information about the operations.
    """
    # Check if the directory exists
    if not os.path.isdir(directory):
        print(f"Error: The directory {directory} does not exist.")
        return

    # Get all files in the directory
    try:
        files = [entry.path for entry in os.scandir(directory) if entry.is_file()]
    except Exception as e:
        print(f"Error accessing files in directory: {e}")
        return

    # Define the pattern that the files should match
    pattern = re.compile(r'\d{8}_\d{6}.*\..*')

    # Filter the files that match the pattern
    files_to_rename = [file for file in files if pattern.match(os.path.basename(file))]

    if not files_to_rename:
        print("No files matching the pattern were found.")
        return

    # Create a progress bar with green color
    pbar = tqdm(total=len(files_to_rename), ncols=70, bar_format="{l_bar}%s{bar}%s{r_bar}" % (Fore.GREEN, Style.RESET_ALL))

    # Renamed file counter
    file_count = 0

    for file in files_to_rename:
        try:
            # Extract the base name of the file and the file extension
            base_name, ext = os.path.splitext(os.path.basename(file))
            # Remove anything after HHMMSS from the base name
            base_name = re.sub(r'(\d{8}_\d{6}).*', r'\1', base_name)
            # Extract the date and time from the base name
            date, time = base_name.split('_')
            # Format the date and time
            formatted_date = f"{date[:4]}-{date[4:6]}-{date[6:8]}"
            formatted_time = f"{time[:2]}_{time[2:4]}_{time[4:6]}"
            # Create the new file name
            file_count += 1
            new_name = f"{formatted_date}T{formatted_time}-{file_count:06}{ext.lower()}"
            # Create the new file path
            new_path = os.path.join(directory, new_name)
            # Check if the new file name already exists
            if os.path.exists(new_path):
                print(f"Error: The file {new_path} already exists.")
                continue
            if test_mode or verbose:
                print(file + " --> " + new_path)
            if not test_mode:
                os.rename(file, new_path)
            # Update the progress bar
            pbar.update(1)
        except Exception as e:
            print(f"Error renaming file {file}: {e}")

    # Close the progress bar
    pbar.close()

def main():
    """
    The main function that gets called when the script is run from the command line.
    Prompts the user for the input directory and options if not provided.
    """
    args = sys.argv[1:]

    if args and args[0].startswith('-'):
        switches = args[0]
        directory = args[1] if len(args) > 1 else input("Please enter the directory where the photos are located: ")

        test_mode = 't' in switches
        verbose = 'v' in switches
    else:
        directory = input("Please enter the directory where the photos are located: ")
        test_mode_input = input("Do you want to run in test mode? (y/n): ").lower()
        verbose_input = input("Do you want to display detailed output? (y/n): ").lower()

        test_mode = test_mode_input == 'y'
        verbose = verbose_input == 'y'

    # Call the rename_photos function with the directory, test_mode, and verbose
    rename_photos(directory, test_mode, verbose)

if __name__ == "__main__":
    main()

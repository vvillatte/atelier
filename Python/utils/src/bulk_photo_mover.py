import os
import re
import sys
import hashlib
from tqdm import tqdm
from colorama import Fore, Style, init

# Initialize colorama
init(autoreset=True)

def calculate_checksum(file_path, algorithm='md5'):
    """
    Calculate the checksum of a file using the specified algorithm.

    Args:
        file_path (str): Path to the file.
        algorithm (str): Hash algorithm to use (default is 'sha256').

    Returns:
        str: The calculated checksum.
    """
    hash_alg = hashlib.new(algorithm)
    try:
        with open(file_path, 'rb') as f:
            for chunk in iter(lambda: f.read(4096), b""):
                hash_alg.update(chunk)
    except Exception as e:
        print(f"Error calculating checksum for {file_path}: {e}")
        return None
    return hash_alg.hexdigest()

def move_photos(src_directory, dest_directory, test_mode=False, verbose=False):
    """
    Move photos from the source directory to the destination directory,
    organizing them into subfolders based on the year and month extracted
    from the filenames.

    Args:
        src_directory (str): Path to the source directory containing photos.
        dest_directory (str): Path to the destination directory.
        test_mode (bool): If True, print operations without making changes.
        verbose (bool): If True, print detailed information about the operations.
    """
    if not os.path.isdir(src_directory):
        print(f"Error: The source directory {src_directory} does not exist.")
        return

    if not os.path.exists(dest_directory):
        try:
            os.makedirs(dest_directory)
        except Exception as e:
            print(f"Error creating destination directory {dest_directory}: {e}")
            return

    try:
        files = [entry.path for entry in os.scandir(src_directory) if entry.is_file()]
    except Exception as e:
        print(f"Error accessing files in source directory: {e}")
        return

    valid_extensions = {'.jpg', '.jpeg', '.png', '.mpg', '.mp4', '.avi'}
    pattern = re.compile(r'(\d{4})-(\d{2})-(\d{2})T(\d{2})_(\d{2})_(\d{2})-\d{6}.*\..*')
    files_to_move = [file for file in files if pattern.match(os.path.basename(file)) and os.path.splitext(file)[1].lower() in valid_extensions]

    if not files_to_move:
        print("No files matching the pattern were found.")
        return

    pbar = tqdm(total=len(files_to_move), ncols=70, bar_format="{l_bar}%s{bar}%s{r_bar}" % (Fore.GREEN, Style.RESET_ALL))

    for file in files_to_move:
        try:
            file_name = os.path.basename(file)
            match = pattern.match(file_name)
            if not match:
                continue

            year, month, day, hour, minute, second = match.groups()
            subfolder_path = os.path.join(dest_directory, year, month)

            if not os.path.exists(subfolder_path):
                try:
                    os.makedirs(subfolder_path)
                except Exception as e:
                    print(f"Error creating subfolder {subfolder_path}: {e}")
                    continue

            dest_file_path = os.path.join(subfolder_path, file_name)
            dest_file_path = os.path.splitext(dest_file_path)[0] + os.path.splitext(dest_file_path)[1].lower()

            if os.path.exists(dest_file_path):
                if (os.path.getsize(file) == os.path.getsize(dest_file_path) and
                        calculate_checksum(file) == calculate_checksum(dest_file_path)):
                    if verbose:
                        print(f"Duplicate file found and discarded: {file}")
                    continue

                counter = 1
                new_file_name = os.path.splitext(file_name)[0] + f"_{counter:02}" + os.path.splitext(file_name)[1].lower()
                new_dest_file_path = os.path.join(subfolder_path, new_file_name)
                while os.path.exists(new_dest_file_path):
                    counter += 1
                    new_file_name = os.path.splitext(file_name)[0] + f"_{counter:02}" + os.path.splitext(file_name)[1].lower()
                    new_dest_file_path = os.path.join(subfolder_path, new_file_name)
                dest_file_path = new_dest_file_path

            if test_mode or verbose:
                print(file + " --> " + dest_file_path)
            if not test_mode:
                os.rename(file, dest_file_path)

            pbar.update(1)
        except Exception as e:
            print(f"Error moving file {file}: {e}")

    pbar.close()

def main():
    """
    The main function that gets called when the script is run from the command line.
    Prompts the user for source and destination directories and test mode option if not provided.
    """
    args = sys.argv[1:]

    if args and args[0].startswith('-'):
        switches = args[0]
        src_directory = args[1] if len(args) > 1 else input("Please enter the source directory: ")
        dest_directory = args[2] if len(args) > 2 else input("Please enter the destination directory: ")

        test_mode = 't' in switches
        verbose = 'v' in switches
    else:
        src_directory = input("Please enter the source directory: ")
        dest_directory = input("Please enter the destination directory: ")
        test_mode_input = input("Do you want to run in test mode? (y/n): ").lower()
        verbose_input = input("Do you want to display detailed output? (y/n): ").lower()

        test_mode = test_mode_input == 'y'
        verbose = verbose_input == 'y'

    move_photos(src_directory, dest_directory, test_mode, verbose)

if __name__ == "__main__":
    main()
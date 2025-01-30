import os
import hashlib
import argparse

def get_file_md5(file_path):
    """
    Calculate and return the MD5 checksum of a file.

    Parameters:
    file_path (str): Path to the file.

    Returns:
    str: MD5 checksum of the file.
    """
    hash_md5 = hashlib.md5()
    try:
        with open(file_path, 'rb') as f:
            for chunk in iter(lambda: f.read(4096), b""):
                hash_md5.update(chunk)
    except FileNotFoundError:
        print(f"Error: File not found - {file_path}")
    except PermissionError:
        print(f"Error: Permission denied - {file_path}")
    return hash_md5.hexdigest()

def find_and_remove_duplicates(folder_path, test_mode=False, verbosity=False):
    """
    Find and remove duplicate image files in a folder based on filename and MD5 checksum.

    Parameters:
    folder_path (str): Path to the folder containing the images.
    test_mode (bool): If True, only outputs results without deleting files.
    verbosity (bool): If True, outputs results of the actions taken.

    Returns:
    None
    """
    files_by_datetime = {}

    try:
        for filename in os.listdir(folder_path):
            if not filename.endswith(('.jpg', '.jpeg', '.png', '.gif', '.bmp', '.mpg', '.mp4', '.avi', '.mov')):
                continue

            datetime_part = filename[:19]  # Extract the YYYY-MM-DDTHH_mm_ss part
            if datetime_part not in files_by_datetime:
                files_by_datetime[datetime_part] = []
            files_by_datetime[datetime_part].append(filename)

        for datetime_part, files in files_by_datetime.items():
            if len(files) > 1:
                checksums = {}
                for file in files:
                    file_path = os.path.join(folder_path, file)
                    file_checksum = get_file_md5(file_path)
                    if file_checksum in checksums:
                        if not test_mode:
                            try:
                                os.remove(file_path)
                                if verbosity:
                                    print(f"Removed duplicate file: {file_path}")
                            except FileNotFoundError:
                                print(f"Error: File not found - {file_path}")
                            except PermissionError:
                                print(f"Error: Permission denied - {file_path}")
                        else:
                            print(f"Duplicate file detected (test mode): {file_path}")
                    else:
                        checksums[file_checksum] = file
    except FileNotFoundError:
        print(f"Error: Folder not found - {folder_path}")
    except PermissionError:
        print(f"Error: Permission denied - {folder_path}")

def main():
    """
    Main function to parse command-line arguments and invoke the find_and_remove_duplicates function.

    Returns:
    None
    """
    parser = argparse.ArgumentParser(description='Remove duplicate images based on filename and MD5 checksum.')
    parser.add_argument('folder_path', type=str, help='Path to the folder containing the images')
    parser.add_argument('-t', '--test', action='store_true', help='Test mode (only output results without deleting files)')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose mode (output results)')

    args = parser.parse_args()
    find_and_remove_duplicates(args.folder_path, test_mode=args.test, verbosity=args.verbose)

if __name__ == '__main__':
    main()

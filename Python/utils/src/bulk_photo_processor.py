import argparse
import os
from bulk_photo_renamer import rename_photos
from bulk_photo_mover import move_photos

def process_photos(source_folder, target_folder, test_mode, verbose):
    """
    Process photos by renaming and moving them.

    This function performs the following tasks:
    1. Validates the source and target folder paths.
    2. Runs the renaming and moving functions in sequence.

    Parameters:
    source_folder (str): Path to the source folder containing the images.
    target_folder (str): Path to the target folder.
    test_mode (bool): Test mode (only output results without renaming or moving files).
    verbose (bool): Verbose mode (output results).

    Returns:
    None
    """
    # Validate the source folder
    if not os.path.isdir(source_folder):
        print(f"Error: The source folder '{source_folder}' does not exist.")
        return

    # Validate the target folder
    if not os.path.isdir(target_folder):
        try:
            os.makedirs(target_folder)
            if verbose:
                print(f"Created target folder: {target_folder}")
        except Exception as e:
            print(f"Error creating target folder '{target_folder}': {e}")
            return

    try:
        # Run the renamer function
        rename_photos(source_folder, test_mode=test_mode, verbose=verbose)

        # Run the mover function
        move_photos(source_folder, target_folder, test_mode=test_mode, verbose=verbose)

    except Exception as e:
        print(f"An unexpected error occurred: {e}")

def main():
    """
    Main function to parse command-line arguments and invoke the bulk photo renamer
    and mover functions, or prompt the user for input if arguments are missing.

    This function performs the following tasks:
    1. Parses command-line arguments to get the source and target folder paths, as well as optional switches for test mode and verbosity.
    2. Prompts the user for input if arguments are missing.
    3. Calls the process_photos function to run the renaming and moving functions.

    Command-line arguments:
    -t, --test (bool): Test mode (only output results without renaming or moving files).
    -v, --verbose (bool): Verbose mode (output results).
    source_folder (str): Path to the source folder containing the images.
    target_folder (str): Path to the target folder.

    Returns:
    None
    """
    parser = argparse.ArgumentParser(description='Run bulk photo renamer and mover scripts.')
    parser.add_argument('-t', '--test', action='store_true', help='Test mode (only output results without renaming or moving files)')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose mode (output results)')
    parser.add_argument('source_folder', type=str, nargs='?', help='Path to the source folder containing the images')
    parser.add_argument('target_folder', type=str, nargs='?', help='Path to the target folder')

    args = parser.parse_args()

    # If no arguments were provided at all, prompt the user for input
    if not any(vars(args).values()):
        args.source_folder = input("Please enter the source folder: ")
        args.target_folder = input("Please enter the target folder: ")
        test_mode_input = input("Do you want to run in test mode? (y/n): ").lower()
        args.test = test_mode_input == 'y'
        verbose_input = input("Do you want to display detailed output? (y/n): ").lower()
        args.verbose = verbose_input == 'y'
    else:
        # Use default values if the optional switches are not provided
        if args.source_folder is None:
            args.source_folder = input("Please enter the source folder: ")
        if args.target_folder is None:
            args.target_folder = input("Please enter the target folder: ")

    process_photos(args.source_folder, args.target_folder, args.test, args.verbose)

if __name__ == '__main__':
    main()

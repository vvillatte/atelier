import argparse
import os
from bulk_photo_renamer import rename_photos
from bulk_photo_mover import move_photos

def main():
    """
    Main function to parse command-line arguments and invoke the bulk photo renamer
    and mover functions.

    This function performs the following tasks:
    1. Parses command-line arguments to get the source and target folder paths, as well as optional switched for test mode and verbosity.
    2. Validates the source and target folder paths.
    3. Runs the renaming and moving functions in sequence.

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
    parser.add_argument('source_folder', type=str, help='Path to the source folder containing the images')
    parser.add_argument('target_folder', type=str, help='Path to the target folder')

    args = parser.parse_args()

    # Validate the source folder
    if not os.path.isdir(args.source_folder):
        print(f"Error: The source folder '{args.source_folder}' does not exist.")
        return

    # Validate the target folder
    if not os.path.isdir(args.target_folder):
        try:
            os.makedirs(args.target_folder)
            if args.verbose:
                print(f"Created target folder: {args.target_folder}")
        except Exception as e:
            print(f"Error creating target folder '{args.target_folder}': {e}")
            return

    try:
        # Run the renamer function
        rename_photos(args.source_folder, test_mode=args.test, verbose=args.verbose)

        # Run the mover function
        move_photos(args.source_folder, args.target_folder, test_mode=args.test, verbose=args.verbose)

    except Exception as e:
        print(f"An unexpected error occurred: {e}")

if __name__ == '__main__':
    main()

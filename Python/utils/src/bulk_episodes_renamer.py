import os
import re

def rename_episodes(directory, pattern, new_name_format):
    """
    This function renames the files in the given directory based on the specified pattern and new name format.

    Args:
    directory (str): The directory where the files are located.
    pattern (str): The regex pattern to match the filenames.
    new_name_format (str): The format for the new filename.
    """

    for filename in os.listdir(directory):
        if re.match(pattern, filename):
            match = re.search(pattern, filename)
            if match:
                new_filename = new_name_format.format(**match.groupdict())
                os.rename(os.path.join(directory, filename), os.path.join(directory, new_filename))
                print(f"{filename} --> {new_filename}")


def main():
    directory = input("Enter the directory path: ")
    pattern = input("Enter the regex pattern to match filenames: ")
    new_name_format = input("Enter the new name format (use {group_name} for captured groups): ")
    rename_episodes(directory, pattern, new_name_format)


if __name__ == "__main__":
    main()

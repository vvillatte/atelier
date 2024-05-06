import os
import re

# Define the directory where the files are located
directory = "I:\\Dump"

def rename_files(directory):
    """
    This function renames the files in the given directory based on certain patterns.
    
    Args:
    directory (str): The directory where the files are located.
    """
    
    # # Iterate over all files in the directory
    # for filename in os.listdir(directory):
    #     # If the filename matches the pattern
    #     if re.match(r"Grey's Anatomy \(2005\) - S\d{2}E\d{2} - .+ \(1080p AMZN WEB-DL x265 Garshasp\).mkv", filename):
    #         # Extract the season and episode number and the episode title
    #         season_episode, title = re.search(r"S\d{2}E\d{2} - .+ (?=\(1080p AMZN WEB-DL x265 Garshasp\).mkv)", filename).group().split(" - ", 1)
    #         # Define the new filename
    #         new_filename = f"{season_episode} - {title.rstrip()}.mkv"
    #         # Rename the file
    #         print(f"{filename} --> {new_filename}")
    #         os.rename(os.path.join(directory, filename), os.path.join(directory, new_filename))


    # # Iterate over the files in the directory
    # for filename in os.listdir(directory):
    #     # If the filename matches the pattern
    #     # if re.match(r'\[NewbSubs\] Shingeki no Kyojin \d{2} \(1080p Blu-ray x265 Dual Audio\)\[\w{8}\]', filename):
    #     if re.match(r'\[NewbSubs\] Shingeki no Kyojin S4 \d{2} \(1080p x265 AAC\)\[\w{8}\].mkv', filename):
    #         # Extract the episode number
    #         episode_number = re.search(r'\d{2}', filename).group()

    #         # Construct the new filename
    #         # new_filename = f"Shingeki no Kyojin S01E{episode_number}.mkv"
    #         new_filename = f"Shingeki no Kyojin S04E{episode_number}.mkv"

    #         # Rename the file
    #         print(f"{filename} --> {new_filename}\r")
    #         os.rename(os.path.join(directory, filename), os.path.join(directory, new_filename))

    # # Print a success message
    # print("Files have been successfully renamed.")

    for filename in os.listdir(directory):
        if filename.endswith(".mkv"):
            # Extract the episode number
            episode_num = re.search(r'\d+', filename).group()
            # Format the episode number to have leading zeros
            episode_num = episode_num.zfill(3)
            # Construct the new filename
            new_filename = f"One Piece S01E{episode_num}.mkv"
            # Get the full original file path
            original_file = os.path.join(directory, filename)
            # Get the full new file path
            new_file = os.path.join(directory, new_filename)
            # Rename the file
            # os.rename(original_file, new_file)
            print(f"{original_file} --> {new_file}")

    # Print a success message
    print("Files have been successfully renamed.")

rename_files(directory)
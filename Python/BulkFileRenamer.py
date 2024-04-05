import os
import re

# # Define the directory where the files are located
# directory = "I:\\Dump\\Grey's Anatomy (2005) Season 17 S17 (1080p AMZN WEB-DL x265 HEVC 10bit EAC3 5.1 Garshasp)"

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

# Define the directory where the files are located
directory = "D:\\home\\vvillatte\\Torrents\\Attack on Titan\\Attack on Titan - Shingeki no Kyojin\\6. Attack on Titan S4 (2020)"

# Iterate over the files in the directory
for filename in os.listdir(directory):
    # If the filename matches the pattern
    # if re.match(r'\[NewbSubs\] Shingeki no Kyojin \d{2} \(1080p Blu-ray x265 Dual Audio\)\[\w{8}\]', filename):
    if re.match(r'\[NewbSubs\] Shingeki no Kyojin S4 \d{2} \(1080p x265 AAC\)\[\w{8}\].mkv', filename):
        # Extract the episode number
        episode_number = re.search(r'\d{2}', filename).group()

        # Construct the new filename
        # new_filename = f"Shingeki no Kyojin S01E{episode_number}.mkv"
        new_filename = f"Shingeki no Kyojin S04E{episode_number}.mkv"

        # Rename the file
        print(f"{filename} --> {new_filename}\r")
        os.rename(os.path.join(directory, filename), os.path.join(directory, new_filename))

# Print a success message
print("Files have been successfully renamed.")
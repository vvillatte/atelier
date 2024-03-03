"""
Author: Vincent Villatte
Version: 1.0
Date: 22/02/2024

This script downloads a KMZ file from a specific webpage and stores it in a 'data' folder.
The script is designed to be run as a standalone program or imported as a module in another script.

Usage:
    python3 downloadKMZ.py
"""

import requests
from bs4 import BeautifulSoup
import re
import os
import zipfile
import platform

def download_file(url, filename):
    """
    Downloads a file from a given URL and saves it to a specified filename.
    Returns True if the file was downloaded successfully, or False otherwise.
    """
    response = requests.get(url)
    if response.status_code == 200:
        os.makedirs(os.path.dirname(filename), exist_ok=True)
        with open(filename, 'wb') as file:
            file.write(response.content)
        return True
    else:
        return False

def downloadKMZ():
    """
    Downloads a KMZ file from a specific webpage and stores it in a 'data' folder.
    Returns the absolute path of the downloaded file.
    """
    # Base URL of the webpage where the files are listed
    base_url = "https://nats-uk.ead-it.com"

    print(f"Parsing {base_url}/cms-nats/opencms/en/uas-restriction-zones/")
    response = requests.get(f"{base_url}/cms-nats/opencms/en/uas-restriction-zones/")
    soup = BeautifulSoup(response.text, 'html.parser')

    # Find the link that matches the filename pattern
    for link in soup.find_all('a', href=True):
        if re.match(r'/cms-nats/opencms/en/Publications/digital-datasets/drone-map/export-eaip3d-\d{8}-CRC_.*\.kmz$', link['href']):
            filename = link['href'].split('/')[-1]
            print(f"File name {filename}")
            file_url = f"{base_url}{link['href']}"

            # Check what operating system the script is run on
            platform_name = platform.system()
            if('Linux' == platform_name):
                filepath = os.path.join("..", "data", filename)
            elif('Windows' == platform_name):
                filepath = os.path.join(os.path.dirname(os.path.realpath(__file__)), "..", "data", filename)
            elif('Darwin' == platform_name):
                filepath = os.path.join("..", "data", filename)
            else:
                print("Unknown Operating System - Aborting")
                return "file not found"
            
            if download_file(file_url, filepath):
                print(f"File {filename} downloaded successfully.")
                return os.path.abspath(filepath)
            else:
                print(f"Failed to download file {filename}.")
            break  # Stop after downloading the first matching file

def extract_kmz(kmz_path, output_path):
    """
    Extracts the contents of a KMZ file to a specified directory.

    Args:
        kmz_path (str): The path to the KMZ file.
        output_path (str): The directory where the contents of the KMZ file will be extracted.
    """
    with zipfile.ZipFile(kmz_path, 'r') as kmz:
        kmz.extractall(output_path)

def main():
    """
    Main function that calls the downloadKMZ function and prints the file path.
    """
    filepath = downloadKMZ()
    print(f"File path: {filepath}")

if __name__ == "__main__":
    main()
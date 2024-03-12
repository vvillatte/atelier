"""
Author: Vincent Villatte
Version: 1.0
Date: 22/02/2024

This script calls the downloadXML and downloadKMZ functions to download the latest files from NATS UK and stores them in a 'data' folder.
This script then extracts the content of the KMZ file (KML file) in the same folder.
The script is designed to be run as a standalone program or imported as a module in another script.

Usage:
    python3 downloadKMZ.py
"""

import os
import downloadXML
import downloadKMZ

def main():
    # Call the downloadXML function and get the file path
    XML_file_path = downloadXML.downloadXML()
    print(f"File path: {XML_file_path}")

    # Call the downloadKMZ function and get the file path
    kmz_file_path = downloadKMZ.downloadKMZ()
    print(f"KMZ file path: {kmz_file_path}")

    # Extract the KMZ file to the same directory
    output_path = os.path.dirname(kmz_file_path)
    downloadKMZ.extract_kmz(kmz_file_path, output_path)
    print(f"KMZ file extracted to: {output_path}")

if __name__ == "__main__":
    main()

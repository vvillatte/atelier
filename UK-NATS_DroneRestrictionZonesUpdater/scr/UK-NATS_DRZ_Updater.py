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

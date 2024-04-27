from PyPDF2 import PdfMerger
import os

def merge_pdfs_in_directory(directory_path, output_filename="merged_result.pdf"):
    """
    Merges all PDF files within a specified directory.

    Args:
        directory_path (str): Path to the folder containing PDF files.
        output_filename (str, optional): Name of the merged output file. Defaults to "merged_result.pdf".
    """
    merger = PdfMerger()

    for root, _, files in os.walk(directory_path):
        for filename in files:
            if filename.lower().endswith(".pdf"):
                pdf_path = os.path.join(root, filename)
                merger.append(pdf_path)

    # Save the merged output in the same folder as the input PDFs
    output_path = os.path.join(directory_path, output_filename)
    merger.write(output_path)
    merger.close()

def main():
    """
    Main function to prompt user input and merge PDFs.
    """
    try:
        directory_to_merge = input("Enter the path to the folder containing PDF files: ")
        output_filename = input("Enter the desired output filename (e.g., merged_output.pdf): ")
        merge_pdfs_in_directory(directory_to_merge, output_filename)
        print(f"PDFs merged successfully into {output_filename}")
    except Exception as e:
        print(f"An error occurred: {str(e)}")

if __name__ == "__main__":
    main()

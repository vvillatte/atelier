import xml.etree.ElementTree as ET

def parse_kml(file_path):
    """
    Parse a KML file and return the root element.

    Args:
        file_path (str): The path to the KML file.

    Returns:
        Element: The root element of the KML file.
    """
    try:
        tree = ET.parse(file_path)
        root = tree.getroot()
        return root
    except ET.ParseError as e:
        print(f"Error parsing KML file: {e}")
        return None
    except FileNotFoundError:
        print("KML file not found.")
        return None

def parse_aixm(file_path):
    """
    Parse an AIXM file and return the root element.

    Args:
        file_path (str): The path to the AIXM file.

    Returns:
        Element: The root element of the AIXM file.
    """
    try:
        tree = ET.parse(file_path)
        root = tree.getroot()
        return root
    except ET.ParseError as e:
        print(f"Error parsing AIXM file: {e}")
        return None
    except FileNotFoundError:
        print("AIXM file not found.")
        return None

def compare_elements(kml_root, aixm_root):
    """
    Compare elements between KML and AIXM files.

    Args:
        kml_root (Element): The root element of the KML file.
        aixm_root (Element): The root element of the AIXM file.

    Returns:
        tuple: A tuple containing sets of common elements, elements only in KML, and elements only in AIXM.
    """
    if kml_root is None or aixm_root is None:
        print("One or both of the XML roots are None.")
        return set(), set(), set()

    kml_elements = {elem.tag for elem in kml_root.iter()}
    aixm_elements = {elem.tag for elem in aixm_root.iter()}

    common_elements = kml_elements & aixm_elements
    kml_only_elements = kml_elements - aixm_elements
    aixm_only_elements = aixm_elements - kml_elements

    return common_elements, kml_only_elements, aixm_only_elements

def main(kml_file, aixm_file):
    """
    Main function to parse KML and AIXM files and compare their elements.

    Args:
        kml_file (str): The path to the KML file.
        aixm_file (str): The path to the AIXM file.
    """
    kml_root = parse_kml(kml_file)
    aixm_root = parse_aixm(aixm_file)

    common_elements, kml_only_elements, aixm_only_elements = compare_elements(kml_root, aixm_root)

    print("Common elements:")
    for elem in common_elements:
        print(elem)

    print("\nElements only in KML file:")
    for elem in kml_only_elements:
        print(elem)

    print("\nElements only in AIXM file:")
    for elem in aixm_only_elements:
        print(elem)

if __name__ == "__main__":
    kml_file = "C:/tmp/WEF2024-09-05_EXP2024-07-08_CRC_4A961A4C.kml"
    aixm_file = "C:/tmp/WEF2024-09-05_EXP2024-07-08_CRC_4A961A4C.xml"
    main(kml_file, aixm_file)

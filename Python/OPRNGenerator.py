import random
import string

def is_valid_country_code(country_code):
    """
    Checks if the given country code is valid.

    Args:
        country_code (str): The country code to check.

    Returns:
        bool: True if the country code is valid, False otherwise.
    """
    # List of ISO 3166 Alpha-3 country codes (European countries only)
    # valid_codes = ["AFG", "ALA", "ALB", "DZA", "ASM", "AND", "AGO", "AIA", "ATA", "ATG", "ARG", "ARM", "ABW", "AUS", "AUT", "AZE", "BHS", "BHR", "BGD", "BRB", "BLR", "BEL", "BLZ", "BEN", "BMU", "BTN", "BOL", "BES", "BIH", "BWA", "BVT", "BRA", "IOT", "BRN", "BGR", "BFA", "BDI", "CPV", "KHM", "CMR", "CAN", "CYM", "CAF", "TCD", "CHL", "CHN", "CXR", "CCK", "COL", "COM", "COG", "COD", "COK", "CRI", "CIV", "HRV", "CUB", "CUW", "CYP", "CZE", "DNK", "DJI", "DMA", "DOM", "ECU", "EGY", "SLV", "GNQ", "ERI", "EST", "SWZ", "ETH", "FLK", "FRO", "FJI", "FIN", "FRA", "GUF", "PYF", "ATF", "GAB", "GMB", "GEO", "DEU", "GHA", "GIB", "GRC", "GRL", "GRD", "GLP", "GUM", "GTM", "GGY", "GIN", "GNB", "GUY", "HTI", "HMD", "VAT", "HND", "HKG", "HUN", "ISL", "IND", "IDN", "IRN", "IRQ", "IRL", "IMN", "ISR", "ITA", "JAM", "JPN", "JEY", "JOR", "KAZ", "KEN", "KIR", "PRK", "KOR", "KWT", "KGZ", "LAO", "LVA", "LBN", "LSO", "LBR", "LBY", "LIE", "LTU", "LUX", "MAC", "MKD", "MDG", "MWI", "MYS", "MDV", "MLI", "MLT", "MHL", "MTQ", "MRT", "MUS", "MYT", "MEX", "FSM", "MDA", "MCO", "MNG", "MNE", "MSR", "MAR", "MOZ", "MMR", "NAM", "NRU", "NPL", "NLD", "NCL", "NZL", "NIC", "NER", "NGA", "NIU", "NFK", "MNP", "NOR", "OMN", "PAK", "PLW", "PSE", "PAN", "PNG", "PRY", "PER", "PHL", "PCN", "POL", "PRT", "PRI", "QAT", "REU", "ROU", "RUS", "RWA", "BLM", "SHN", "KNA", "LCA", "MAF", "SPM", "VCT", "WSM", "SMR", "STP", "SAU", "SEN", "SRB", "SYC", "SLE", "SGP", "SXM", "SVK", "SVN", "SLB", "SOM", "ZAF", "SGS", "SSD", "ESP", "LKA", "SDN", "SUR", "SJM", "SWE", "CHE", "SYR", "TWN", "TJK", "TZA", "THA", "TLS", "TGO", "TKL", "TON", "TTO", "TUN", "TUR", "TKM", "TCA", "TUV", "UGA", "UKR", "ARE", "GBR", "USA", "UMI", "URY", "UZB", "VUT", "VEN", "VNM", "VGB", "VIR", "WLF", "ESH", "YEM", "ZMB", "ZWE"]
    valid_codes = ["ALB", "AND", "AUT", "BEL", "BGR", "BIH", "BLR", "CHE", "CYP", "CZE", "DEU", "DNK", "ESP", "EST", "FIN", "FRA", "GBR", "GRC", "HRV", "HUN", "IRL", "ISL", "ITA", "LIE", "LTU", "LUX", "LVA", "MCO", "MDA", "MKD", "MLT", "MNE", "NLD", "NOR", "POL", "PRT", "ROU", "RUS", "SMR", "SRB", "SVK", "SVN", "SWE", "UKR", "VAT"]
    return country_code in valid_codes

def luhn_modulo_36(input_string):
    """
    Calculates the Luhn Modulo 36 checksum for a given input string.

    Args:
        input_string (str): The input string to calculate the checksum for.

    Returns:
        str: The calculated checksum as a string.
    """
    # Define the character set for modulo 36
    charset = '0123456789abcdefghijklmnopqrstuvwxyz'
    
    # Ensure the input string is 15 characters long
    if len(input_string) != 15:
        return "Input string must be 15 characters long"
    
    # Ensure the input string only contains valid characters
    if not all(char in charset for char in input_string):
        return "Input string contains invalid characters"
    
    # Calculate the Luhn Modulo 36 checksum
    sum = 0
    for i in range(len(input_string)):
        char = input_string[i]
        num = charset.index(char)
        
        # Double every second digit from the right
        if i % 2 == 0:
            num *= 2
        
        # Subtract 9 if the number is greater than 35
        if num > 35:
            num -= 35

        # Add the number to the sum
        sum += num
    
    # Calculate the checksum modulo 36
    checksum = 36 * (sum // 36 + 1) - sum if sum % 36 != 0 else 0

    # Return the equivalent character
    return str(charset[checksum])

def generate_oprn(country_code):
    """
    Generates an OPRN (Operator Public Reference Number) for a given country code.

    Args:
        country_code (str): The country code to generate the OPRN for.

    Returns:
        str: The generated OPRN.
    """
    # Check the country code validity
    if not is_valid_country_code(country_code):
        print("Invalid country code. Please enter a valid ISO 3166 Alpha-3 country code.")
        return

    # Ensure the country code is upper-case
    country_code = country_code.upper()

    # Generate twelve (12) randomly generated characters
    random_chars = ''.join(random.choice(string.ascii_lowercase+string.digits) for _ in range(12))

    # Generate twelve (12) randomly generated characters
    secret = ''.join(random.choice(string.ascii_lowercase+string.digits) for _ in range(3))

    # Generate three (3) randomly generated “secret digits”
    checksum = luhn_modulo_36(random_chars+secret)

    # Combine all parts
    oprn = country_code + random_chars + checksum + "-" + secret

    # Add hyphen at the fourth position from right to left
    return oprn

def main():
    """
    Main function to run the program.
    """
    # Prompt the user for the country code
    country_code = input("Please enter your ISO 3166 Alpha-3 country code: ")

    oprn = generate_oprn(country_code)
    print(oprn)

if __name__ == "__main__":
    main()
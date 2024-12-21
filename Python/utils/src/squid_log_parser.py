"""
This module contains functions to:
1- parse a squid access.log file
2- count the number of occurrences of each second-level domain and TLD
3- and send the results via email.

Functions:
parse_log(file_path): Parse the squid access.log file and count the number of occurrences of each second-level domai
n and TLD.
get_sld_tld(domain): Extract the second-level domain and TLD from a domain.
send_email(from_email, password, to_email, results): Send an email with the results.
main(): Main function to parse the log file, print the results, and send the results via email.
"""

import re
from collections import defaultdict
from urllib.parse import urlparse
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

def parse_log(file_path):
    """
    Parse the squid access.log file and count the number of occurrences of each second-level domain and TLD.

    Args:
    file_path (str): The path to the log file.

    Returns:
    dict: A dictionary where the keys are the second-level domains and TLDs and the values are the counts.
    """
    domain_count = defaultdict(int)
    with open(file_path, 'r') as file:
        for line in file:
            match = re.search(r'CONNECT (.*):443', line)
        if match:
            domain = match.group(1)
            sld_tld = get_sld_tld(domain)
            domain_count[sld_tld] += 1
    return domain_count

def get_sld_tld(domain):
    """
    Extract the second-level domain and TLD from a domain.

    Args:
    domain (str): The domain.

    Returns:
    str: The second-level domain and TLD.
    """
    parsed_uri = urlparse("http://" + domain)
    # domain = '{uri.netloc}'.format(uri=parsed_uri)
    domain = f'{parsed_uri.netloc}'
    sld_tld = '.'.join(domain.split('.')[-2:])
    return sld_tld

def send_email(from_email, password, to_email, results):
    """
    Send an email with the results.

    Args:
    from_email (str): The sender's email address.
    password (str): The sender's email password.
    to_email (str): The recipient's email address.
    results (str): The results to be sent in the email body.
    """
    msg = MIMEMultipart()
    msg['From'] = from_email
    msg['To'] = to_email
    msg['Subject'] = "Squid Access Log Analysis Results"

    body = "Here are the results of the Squid access log analysis:\n\n" + results
    msg.attach(MIMEText(body, 'plain'))

    server = smtplib.SMTP('mail.vvillatte.net', '465')
    server.starttls()
    server.login(from_email, password)
    text = msg.as_string()
    server.sendmail(from_email, to_email, text)
    server.quit()

def main():
    """
    Main function to parse the log file, print the results, and send the results via email.
    """
    file_path = "/var/log/squid/access.log"
    domain_count = parse_log(file_path)

    unsorted_results = "\n".join(f"{domain}: {count}" for domain, count in domain_count.items())
    sorted_domains = sorted(domain_count.items(), key=lambda x: (-x[1], x[0]))
    sorted_results = "\n".join(f"{domain}: {count}" for domain, count in sorted_domains)

    results = f"Unsorted Results:\n{unsorted_results}\n\nSorted Results:\n{sorted_results}"
    print(results)

    from_email = "admin@vvillatte.net"
    password = "password"
    to_email = "vincent@villatte.net"
    send_email(from_email, password, to_email, results)

if __name__ == "__main__":
    main()

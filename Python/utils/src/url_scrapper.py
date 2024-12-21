##
# In this updated script, the scrape_links function now returns the list of URLs instead of printing them.
# The main function then prints each URL on a new line.
# This allows the scrape_links function to be used in other contexts where you might want to do something with the URLs other than just print them.
# For example, you could write them to a file, or use them as input to another function.
# Author: Vincent Villatte (and Microsoft Copilot)
# Version: 1.0 | Date: 12/03/2024
##

import requests
from bs4 import BeautifulSoup

def scrape_links(url):
    """
    This function scrapes all the URLs from a given webpage.

    Parameters:
    url (str): The URL of the webpage to scrape.

    Returns:
    urls (list): A list of URLs found on the page.
    """

    # Send a GET request to the page
    response = requests.get(url)

    # Parse the HTML content of the page using BeautifulSoup
    soup = BeautifulSoup(response.text, 'html.parser')

    # Find all 'a' tags in the HTML, which usually contain the URLs
    links = soup.find_all('a')

    # Extract the href attribute from each 'a' tag. The href attribute usually contains the URL.
    urls = [link.get('href') for link in links if link.get('href') is not None]

    # Return the list of URLs
    return urls

def main():
    url = input("Please enter the URL of the page you want to scrape: ")
    urls = scrape_links(url)
    print("\n".join(urls))

if __name__ == "__main__":
    main()

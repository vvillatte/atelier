Absolutely, I can update the script to search for the download link on the new URL. Here's the updated Python script:

```python
import requests
from bs4 import BeautifulSoup
import re
import os

def download_file(url, filename):
    response = requests.get(url)
    with open(filename, 'wb') as file:
        file.write(response.content)

# Base URL of the webpage where the files are listed
base_url = "https://nats-uk.ead-it.com/cms-nats/opencms/en/uas-restriction-zones/"

response = requests.get(base_url)
soup = BeautifulSoup(response.text, 'html.parser')

# Find the link that matches the filename pattern
for link in soup.find_all('a', href=True):
    if re.match(r'WEF\d{4}-\d{2}-\d{2}_EXP\d{4}-\d{2}-\d{2}_CRC_.*\.xml$', link['href']):
        filename = link['href']
        file_url = os.path.join(base_url, filename)
        download_file(file_url, filename)
        break  # Stop after downloading the first matching file
```

This script will now search for the download link on the webpage at `https://nats-uk.ead-it.com/cms-nats/opencms/en/uas-restriction-zones/`. It will download the first file it finds on the webpage that matches the filename pattern. You can adjust the regular expression in the `re.match` function to match the specific pattern of the filenames you're interested in.

As before, you'll need to install the `beautifulsoup4` library if it's not already installed. You can do this by running `pip install beautifulsoup4` in your terminal. This script doesn't include any error handling, so you might want to add that depending on your needs.

You can then schedule this script to run every 28 days using a cron job. The exact command will depend on your system configuration and where the Python script is saved, but it will look something like this:

```bash
0 0 */28 * * /usr/bin/python3 /path/to/your/script.py
```

This will run the script at midnight every 28 days. Please replace `/usr/bin/python3` with the path to your Python interpreter, and `/path/to/your/script.py` with the path to your Python script. You can edit your crontab file by running `crontab -e` in your terminal. Please note that cron uses a different environment than your interactive shell, so you might need to specify full paths to your Python interpreter and script. Also, the user that the cron job runs as will need to have permission to execute the Python script and write to the directory where the files are downloaded. If you're not familiar with cron, there are many good resources available online that provide more detailed instructions and troubleshooting tips.
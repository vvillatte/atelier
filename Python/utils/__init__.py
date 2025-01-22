# utils/__init__.py

# Initialize package-level configurations
import os
import sys
from datetime import datetime

# Add the source directory to the system path
src_path = os.path.join(os.path.dirname(__file__), 'src')
if src_path not in sys.path:
    sys.path.append(src_path)

# Export selected modules
from src.bulk_episodes_renamer import rename_episodes
from src.bulk_photo_renamer import rename_photos

# Package metadata
__author__ = "Vincent Villatte"
__version__ = "1.0.0"
__description__ = "A utility package with various useful scripts"

# Get the modification time of this file
file_path = os.path.abspath(__file__)
modification_time = os.path.getmtime(file_path)
modification_date = datetime.fromtimestamp(modification_time).strftime("%Y-%m-%d %H:%M:%S")

# Add the modification date to the metadata
__modification_date__ = modification_date

# Set a default configuration for the package
os.environ['UTILS_CONFIG'] = 'default'
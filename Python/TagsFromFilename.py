#/usr/bin/python3
import re, os
from exiftool import ExifToolHelper

# path to EXIF tool
EXIFToolPath = r'C://Users//vvill_p3wv55t//Applications//exiftool-12.48//exiftool.exe'

ImagesPath = r'C://Users//vvill_p3wv55t//Downloads//Imagerie TEREGA - Vol 8 - Vol 2//10_22_03_2020'
print(ImagesPath)
extension = ".jpg"
print(extension)

with ExifToolHelper(EXIFToolPath) as et:
    for root, directories, files in os.walk(ImagesPath):
        for file in files:
            if file.endswith(extension) :
                filename = os.path.join(root, file)
                print(filename)
                m = re.search(r'photoNum_(?P<NUM>[0-9]+)', filename)
                imageNumber = m.group('NUM')
                m = re.search(r'__lat_(?P<LAT>-?[0-9]+\.[0-9]+)', filename)
                latitude = m.group('LAT')
                m = re.search(r'__lon_(?P<LONG>-?[0-9]+\.[0-9]+)', filename)
                longitude = m.group('LONG')
                m = re.search(r'__alt_(?P<ALT>-?[0-9]+\.[0-9]+)', filename)
                altitude = m.group('ALT')
                m = re.search(r'__cap_(?P<CAP>-?[0-9]+\.[0-9]+)', filename)
                capture = m.group('CAP')
                m = re.search(r'__timeGMT_(?P<TIME>[0-9]+_[0-9]+_[0-9]+)', filename)
                GMTTimestamp = m.group('TIME')
                dateCreated = "2022:03:22 "+GMTTimestamp.replace("_", ":")
                dateTimeOriginal = "2022:03:22 "+GMTTimestamp.replace("_", ":")
                try:
                    et.set_tags(
                        filename,
                        tags={"datetimeoriginal": dateTimeOriginal, "GPSLatitude": latitude, "GPSLongitude": longitude, "GPSAltitude": altitude, "CaptureAngle": capture},
                        params=["-P", "-overwrite_original"]
                    )
                    # et.execute("-DateTimeOriginal="+dateTimeOriginal, filename)
                except:
                    print("An error occured")
import csv
import geojson
from geopy.distance import geodesic
import numpy as np

# Define a function to calculate the coordinates of the four corners of a 1-arc-second grid cell
def get_grid_corners(lat, lon):
    d = geodesic((lat, lon), (lat + 1/3600, lon)).meters  # 1 arc-second distance in meters at latitude
    lat1 = lat - (d / 2.0 / 111319.9)  # Convert meters to decimal degrees
    lat2 = lat + (d / 2.0 / 111319.9)
    lon1 = lon - (d / 2.0 / (111319.9 * abs(np.cos(np.radians(lat)))))  # Convert meters to decimal degrees
    lon2 = lon + (d / 2.0 / (111319.9 * abs(np.cos(np.radians(lat)))))
    return [(lon1, lat1), (lon2, lat1), (lon2, lat2), (lon1, lat2), (lon1, lat1)]

# Open the CSV file and read the data
with open('C:/tmp/input.csv', 'r') as csvfile:
    reader = csv.DictReader(csvfile)
    data = list(reader)

# Define the GeoJSON features
features = []
for row in data:
    latitude = float(row['latitude'])
    longitude = float(row['longitude'])
    population = float(row['population_2020'])
    grid_corners = get_grid_corners(latitude, longitude)
    polygon = geojson.Polygon([grid_corners])
    feature = geojson.Feature(geometry=polygon, properties={'population': population})
    features.append(feature)

# Create a GeoJSON FeatureCollection
feature_collection = geojson.FeatureCollection(features)

# Write the GeoJSON data to a new file
with open('C:/tmp/output.geojson', 'w') as outfile:
    geojson.dump(feature_collection, outfile)

import csv
import geojson

# Open the CSV file and read the data
with open('input.csv', 'r') as csvfile:
    reader = csv.DictReader(csvfile)
    data = [row for row in reader]

# Define the GeoJSON features
features = []
for row in data:
    latitude = float(row['latitude'])
    longitude = float(row['longitude'])
    population = float(row['population_2020'])
    point = geojson.Point((longitude, latitude))
    feature = geojson.Feature(geometry=point, properties={'population': population})
    features.append(feature)

# Create a GeoJSON FeatureCollection
feature_collection = geojson.FeatureCollection(features)

# Write the GeoJSON data to a new file
with open('output.geojson', 'w') as outfile:
    geojson.dump(feature_collection, outfile)
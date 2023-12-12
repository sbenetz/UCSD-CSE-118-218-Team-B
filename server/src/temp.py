import sqlite3
import json

from common import *


# Load JSON data from file
with open('temp.json', 'r') as file:
    json_data = file.read()

# Parse JSON data
data = json.loads(json_data)['data']

# Connect to SQLite database (create it if not exists)
conn = sqlite3.connect(PATH_TO_DB)
cursor = conn.cursor()

# Insert data into the database
for entry in data:
    cursor.execute('''
        INSERT INTO deviceLogs (deviceId, timestamp, soilMoisture, sunlight, deviceBattery)
        VALUES (?, ?, ?, ?, ?)
    ''', ('WhQrjTaz0FtWxzUPgX7miKl04', entry['timestamp'], entry['soilMoisture'], entry['sunlight'], entry['deviceBattery']))

# Commit the changes and close the connection
conn.commit()
conn.close()

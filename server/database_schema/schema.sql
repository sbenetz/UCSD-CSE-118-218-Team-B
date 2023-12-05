.mode columns
.headers on
CREATE TABLE IF NOT EXISTS users (
  userId TEXT PRIMARY KEY, 
  username TEXT NOT NULL,
  password TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS devices (
  deviceId TEXT PRIMARY KEY, 
  userId TEXT NOT NULL,
  plantName TEXT NOT NULL,
  plantType INTEGER
);
CREATE TABLE IF NOT EXISTS deviceLogs (
  logId INTEGER PRIMARY KEY AUTOINCREMENT,
  deviceId TEXT,
  timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  soilMoisture INTEGER,
  sunlight INTEGER
);


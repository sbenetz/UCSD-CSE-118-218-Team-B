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
  plantType INTEGER,
  plantSize INTEGER
);
CREATE TABLE IF NOT EXISTS deviceLogs (
  logId INTEGER PRIMARY KEY AUTOINCREMENT,
  deviceId TEXT,
  timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  soilMoisture INTEGER,
  sunlight INTEGER
);
CREATE TABLE IF NOT EXISTS waterLogs (
  logId INTEGER PRIMARY KEY AUTOINCREMENT,
  deviceId TEXT,
  timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE plantTypes (
  typeId INTEGER, -- id of the plant type
  size INTEGER, -- 0: sm, 1: md, 2: lg
  typeName TEXT, -- name of the plant type
  wateringType INTEGER, -- 0: soil moisture based, 1: time based
  soilMoistThresh INTEGER, -- Soil Moisture Threshold
  soilMoistGoal INTEGER, -- Soil Moisture Goal
  waterFreqHrs INTEGER, -- Watering Frequency (hours) - how often to water in hours

  PRIMARY KEY (typeId, size)
);
-- PLANT TYPES --
-- Succulent
INSERT INTO plantTypes VALUES (0,0,'Succulent',1,70,80,336);
INSERT INTO plantTypes VALUES (0,1,'Succulent',1,68,82,336);
INSERT INTO plantTypes VALUES (0,2,'Succulent',1,66,84,336);
-- Flowering Plant
INSERT INTO plantTypes VALUES (1,0,'Flowering Plant',0,70,80,0);
INSERT INTO plantTypes VALUES (1,1,'Flowering Plant',0,68,82,0);
INSERT INTO plantTypes VALUES (1,2,'Flowering Plant',0,66,84,0);
-- Herb
INSERT INTO plantTypes VALUES (2,0,'Herb',0,70,80,0);
INSERT INTO plantTypes VALUES (2,1,'Herb',0,68,82,0);
INSERT INTO plantTypes VALUES (2,2,'Herb',0,66,84,0);
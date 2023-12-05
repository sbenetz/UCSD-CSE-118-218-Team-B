.mode columns
.headers on
CREATE TABLE users (
  userId TEXT PRIMARY KEY, 
  username TEXT NOT NULL,
  password TEXT NOT NULL
);
CREATE TABLE devices (
  deviceId TEXT PRIMARY KEY, 
  userId TEXT NOT NULL,
  plantName TEXT NOT NULL,
  plantType INTEGER
);


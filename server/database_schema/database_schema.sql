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
  plantType TEXT
);

-- users 
insert into users values ('38a8vsd8a84d99', 'frank', '1234');
insert into users values ('quela8928lkdf', 'jeff', '4321');

-- devices
insert into devices values ('queidk7393jd', '38a8vsd8a84d99', 'My Cactus', '0');
insert into devices values ('lkjaeli7s0', '38a8vsd8a84d99', 'My Fig Tree', '1');
insert into devices values ('lkje893ls', 'quela8928lkdf', 'My Vine', '2');


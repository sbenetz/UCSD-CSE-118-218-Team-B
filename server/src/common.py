from pydantic import BaseModel
from typing import List

# -- Constants --
# Database
DB_DIRECTORY = 'database'
DB_FILE_NAME = 'database.db'
PATH_TO_DB = f"../{DB_DIRECTORY}/{DB_FILE_NAME}"

# Device Control
#DEVICE_SLEEP_TIME_S = 1800 # 30 min
DEVICE_SLEEP_TIME_S = 15 # 15 seconds


# - POST Request Bodies -
class Credentials(BaseModel):
  username: str
  password: str

class DeviceInit(BaseModel):
  userId: str
  plantName: str
  plantType: int
  plantSize: int # 0: sm, 1: md, 2: lg

class DeviceCheckIn(BaseModel):
  deviceId: str
  soilMoisture: int
  sunlight: int
  battery: int

class DeviceCredentials(BaseModel):
  deviceId: str

# - Response Bodies -
class UserId(BaseModel):
  userId: str | None = None
  errorMessage: str | None = None

class Plant(BaseModel):
  plantId: str
  plantName: str
  plantType: int
  plantSize: int

class PlantsReturn(BaseModel):
  plants: List[Plant] | None = None
  errorMessage: str | None = None

class SensorDataLog(BaseModel):
  timestamp: str
  soilMoisture: int
  sunlight: int
  deviceBattery: int

class PlantInfoReturn(BaseModel):
  sensorDataLogs: List[SensorDataLog]
  waterHistory: List[str]
  errorMessage: str | None = None

class DeviceCheckInReturn(BaseModel):
  goalMoisture: int
  sleepTime: int

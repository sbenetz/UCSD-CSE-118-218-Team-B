# Main File: API Server
# USAGE: uvicorn main:app --reload --host <ip_address> --port <port_num>
from fastapi import FastAPI

from database import Database
from common import *

# Create FastAPI instance, this is referenced by uvicorn
app = FastAPI()

# Create Database instance
database = Database(PATH_TO_DB)

# Define API Rules
@app.get("/")
async def root() -> str:
  return "Smart Plant Waterer API Server"

# -- PHONE <-> SERVER --
@app.post("/user/new-account")
async def new_account(data: Credentials) -> UserId:
  (userId, errorMessage) = database.create_account(data)
  return {
    "userId": userId,
    "errorMessage": errorMessage
  }
  
@app.post("/user/login")
async def user_login(data: Credentials) -> UserId:
  (userId, errorMessage) = database.login(data)
  return {
    "userId": userId,
    "errorMessage": errorMessage
  }

@app.get("/user/{userId}/plants")
async def user_get_plants(userId) -> PlantsReturn:
  (plants, errorMessage) = database.get_plants(userId)
  return {
    "plants": plants,
    "errorMessage": errorMessage
  }

@app.get("/plants/{plantId}")
async def get_plant(plantId) -> PlantInfoReturn:
  (sensorDataLogs, errorMessage) = database.get_plant_sensor_data_logs(plantId)
  (waterHistory, errorMessage) = database.get_plant_water_history(plantId)
  plantInfo = PlantInfoReturn(sensorDataLogs=sensorDataLogs, waterHistory=waterHistory, errorMessage=errorMessage)
  return plantInfo
  

# -- DEVICE <-> SERVER --
@app.post("/device/initialization")
async def device_initialization(data: DeviceInit) -> str:
  deviceId = database.device_init(data)
  if deviceId is None:
    return ""
  else:
    return deviceId
  
@app.post("/device/check-in")
async def device_checkin(data: DeviceCheckIn) -> int:
  database.device_check_in(data)
  # TODO: implement logic for if to water or not
  return -1

@app.post("/device/water-confirmation")
async def device_water_confirm(data: DeviceCredentials) -> str:
  database.device_water_confirm(data)
  return "ok"

# Remove device from devices & all of it's logs
@app.post("/device/reset")
async def device_reset(data: DeviceCredentials) -> str:
  print(f"/device/reset: {data.deviceId}")
  database.device_reset(data)
  return "ok"
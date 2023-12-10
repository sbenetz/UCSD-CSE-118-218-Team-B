# Main File: API Server
# USAGE: uvicorn main:app --reload --host <ip_address> --port <port_num>
from fastapi import FastAPI

from database import Database
from common import *

from datetime import datetime

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
  print(f"--\n{datetime.now()}")
  print(f"New Account: ({data})")
  (userId, errorMessage) = database.create_account(data)
  return {
    "userId": userId,
    "errorMessage": errorMessage
  }
  
@app.post("/user/login")
async def user_login(data: Credentials) -> UserId:
  print(f"--\n{datetime.now()}")
  print(f"User Login: ({data})")
  (userId, errorMessage) = database.login(data)
  return {
    "userId": userId,
    "errorMessage": errorMessage
  }

@app.get("/user/{userId}/plants")
async def user_get_plants(userId) -> PlantsReturn:
  print(f"--\n{datetime.now()}")
  print(f"Get User's Plants")
  (plants, errorMessage) = database.get_plants(userId)
  plantsReturn = PlantsReturn(plants=plants, errorMessage=errorMessage)
  print(f"Return: {plantsReturn}")
  return plantsReturn

@app.get("/plants/{plantId}")
async def get_plant(plantId) -> PlantInfoReturn:
  print(f"--\n{datetime.now()}")
  print(f"Get Plant Info")
  (sensorDataLogs, errorMessage) = database.get_plant_sensor_data_logs(plantId)
  (waterHistory, errorMessage) = database.get_plant_water_history(plantId)
  plantInfo = PlantInfoReturn(sensorDataLogs=sensorDataLogs, waterHistory=waterHistory, errorMessage=errorMessage)
  print(f"Return info for {plantId}")
  return plantInfo
  

# -- DEVICE <-> SERVER --
@app.post("/device/initialization")
async def device_initialization(data: DeviceInit) -> str:
  print(f"--\nDevice Initialization: ({data})")
  deviceId = database.device_init(data)
  print(f"Return: {deviceId}")
  if deviceId is None:
    return ""
  else:
    return deviceId
  
@app.post("/device/check-in")
async def device_checkin(data: DeviceCheckIn) -> DeviceCheckInReturn:
  print(f"--\n{datetime.now()}")
  print(f"Device Check-in: ({data})")
  database.device_check_in(data)
  goalMoisture = database.water_logic(data) # Logic for if to water or not
  print(f"Return: (goalMoisture: {goalMoisture}, sleepTime: {DEVICE_SLEEP_TIME_S})")
  return DeviceCheckInReturn(goalMoisture=goalMoisture, sleepTime=DEVICE_SLEEP_TIME_S)


# -- CONTROL COMMANDS --
@app.post("/device/force-water") # will tell device to water next time it checks in
async def device_force_water(data: DeviceCredentials) -> str:
  print(f"--\n{datetime.now()}")
  print(f"Device Force Water - deviceId: {data.deviceId}")
  forceWaterList.append(data.deviceId)
  return "ok"

@app.post("/device/reset") # Remove device from devices & all of it's logs
async def device_reset(data: DeviceCredentials) -> str:
  print(f"--\n{datetime.now()}")
  print(f"Device Reset - deviceId: {data.deviceId}")
  database.device_reset(data)
  return "ok"
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
  

@app.post("/device/initialization")
async def device_initialization(data: DeviceInit) -> str:
  deviceId = database.device_init(data)
  return "123-fake-device-id"
  # if deviceId is None:
  #   return ""
  # else:
  #   return "123-fake-device-id"
  
@app.post("/device/check-in")
async def device_checkin(data: DeviceCheckIn) -> int:
  return -1

@app.post("/device/water-confirmation")
async def device_water_confirm(data: DeviceCredentials) -> str:
  return "ok"
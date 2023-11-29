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
async def root():
  return {"info": "Smart Plant Water Server"}

@app.post("/user/new-account")
async def new_account(data: Credentials):
  (userId, errorMessage) = database.create_account(data)
  if errorMessage:
    return response_error(errorMessage)
  else:
    return create_account_response_success(userId)
  
@app.post("/user/login")
async def user_login(data: Credentials):
  (userId, errorMessage) = database.login(data)
  if errorMessage:
    return response_error(errorMessage)
  else:
    return login_response_success(userId)
  

@app.post("/device/initialization")
async def device_initialization(data: DeviceInit):
  deviceId = database.device_init(data)
  return "123-fake-device-id"
  # if deviceId is None:
  #   return ""
  # else:
  #   return "123-fake-device-id"
  
@app.post("/device/check-in")
async def device_checkin(data: DeviceCheckIn):
  return {
    'status': 'nice'
  }
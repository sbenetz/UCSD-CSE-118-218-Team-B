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

@app.post("/create-account")
async def create_account(data: Account):
  (userId, errorMessage) = database.create_account(data)
  if errorMessage:
    return response_error(errorMessage)
  else:
    return create_account_response_success(userId)
  
# @app.post("/login")
# async def login(data: Account):
#   (userId, errorMessage) = database.login(data)
#   if errorMessage:
#     return response_error(errorMessage)
#   else:
#     return login_response_success(userId)

# Temporary
@app.get("/users")
async def users():
  return database.get_users()

@app.get("/devices")
async def devices():
  return database.get_devices()
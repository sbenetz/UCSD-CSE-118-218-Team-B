# USAGE: uvicorn main:app --reload --host <ip_address> --port <port_num>
import uvicorn # server for running FastAPI
from fastapi import FastAPI
import sys # for getting command line arguments

from database import Database

# -- Constants --
# Database
DB_DIRECTORY = 'database'
DB_FILE_NAME = 'database.db'
PATH_TO_DB = f"../{DB_DIRECTORY}/{DB_FILE_NAME}"

# Create FastAPI instance, this is referenced by uvicorn
app = FastAPI()

# Create Database instance
database = Database(PATH_TO_DB)

# Define API Rules
@app.get("/")
async def root():
  return {"info": "Smart Plant Water Server"}

@app.get("/users")
async def users():
  return database.get_users()

@app.get("/devices")
async def devices():
  return database.get_devices()
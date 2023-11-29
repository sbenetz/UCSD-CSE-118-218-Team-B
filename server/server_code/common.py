from pydantic import BaseModel

# -- Constants --
# Database
DB_DIRECTORY = 'database'
DB_FILE_NAME = 'database.db'
PATH_TO_DB = f"../{DB_DIRECTORY}/{DB_FILE_NAME}"




# - POST Request Bodies -
class Credentials(BaseModel):
  username: str
  password: str

class DeviceInit(BaseModel):
  userId: str
  plantName: str
  plantType: int

class DeviceCheckIn(BaseModel):
  deviceId: str
  soilMoisture: int
  sunlight: int

# - Response Bodies -
STATUS_SUCCESS = 'success'
STATUS_ERROR = 'error'

def response_error(error_message: str):
  return {
    STATUS_ERROR: error_message
  }

def create_account_response_success(userId: str):
  return {
    STATUS_SUCCESS: userId
  }

def login_response_success(userId: str):
  return {
    STATUS_SUCCESS: userId
  }

class DeviceId(BaseModel):
  status: str
  deviceId: str 
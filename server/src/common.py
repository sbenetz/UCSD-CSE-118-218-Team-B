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

class DeviceCredentials(BaseModel):
  deviceId: str

# - Response Bodies -
class UserId(BaseModel):
  userId: str | None = None
  errorMessage: str | None = None

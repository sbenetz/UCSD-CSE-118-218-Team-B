# class for interacting with sqlite3 database
from enum import Enum
import sqlite3 # for connecting to SQLite Database
import random, string # for random userId generation

from common import *

# -- Database Schema --
# USERS Table
class USERS:  
  TABLE_NAME = 'users'
  # column names
  USER_ID = 'userId'
  USERNAME = 'username'
  PASSWORD = 'password'

# DEVICES Table
class DEVICES:
  TABLE_NAME = 'devices'
  # column names
  DEVICE_ID = 'deviceId'
  USER_ID = 'userId'
  PLANT_NAME = 'plantName'
  PLANT_TYPE = 'plantType'

# DEVICE LOGS Table
class LOGS:
  TABLE_NAME = 'deviceLogs'
  # column names
  LOG_ID = 'logId'
  DEVICE_ID = 'deviceId'
  TIMESTAMP = 'timestamp'
  SOIL_MOISTURE = 'soilMoisture'
  SUNLIGHT = 'sunlight'

class WATER_LOGS:
  TABLE_NAME = 'waterLogs'
  # column names
  LOG_ID = 'logId'
  DEVICE_ID = 'deviceId'
  TIMESTAMP = 'timestamp'


class Database:
  # -- Public Methods --

  def __init__(self, path_to_db):
    """Constructor"""
    print(f"connecting to database at '{path_to_db}'")
    self.connection = sqlite3.connect(path_to_db)
    self.connection.row_factory = sqlite3.Row
    self.cursor = self.connection.cursor()


  # -- PHONE <-> SERVER --
  def create_account(self, data: Credentials) -> (str, str):
    """Create a new user account with the given credentials. Returns
    the userId for the newly created account.
    Returns: (userId, None) on success, (None, errorMessage) on failure"""
    # TODO: Cleanse username and password
    if not data.username: return (None, "invalid username")
    if not data.password: return (None, "invalid password")

    # username must be unique
    if self.__exists(USERS.TABLE_NAME, USERS.USERNAME, data.username): 
      return (None, "username already exists")

    # Add new entry to Users
    userId = self.__generate_uniq_user_id()
    self.__insert_users(userId, data.username, data.password)
    return (userId, None)


  def login(self, data: Credentials) -> (str, str):
    """Return the userId of the user with the given credentials.
    Returns: (userId, None) on success, (None, errorMessage) on failure"""
    # TODO: Cleanse username and password?
    if not data.username: return (None, "invalid username")
    if not data.password: return (None, "invalid password")

    # get user by username
    params = (data.username, )
    result = self.cursor.execute(f"SELECT * FROM {USERS.TABLE_NAME} WHERE {USERS.USERNAME} = ?", params)
    user = result.fetchone()
    if user is None:
      return (None, "username not found")
    
    # validate password
    if not user[USERS.PASSWORD] == data.password:
      return (None, "wrong password")
    
    # success
    return (user[USERS.USER_ID], None)
  
  def get_plants(self, userId) -> (List[Plant], str):
    """Return a list of all the plants for the given user.
    Returns: (list_of_plants, None) on success, (None, errorMessage) on failure"""

    # Verify userId exists
    if not self.__exists(USERS.TABLE_NAME, USERS.USER_ID, userId):
      return (None, "userId does not exist in users table")
    
    # Get user's plants
    result = self.__get_user_plants(userId)

    # Format Data
    plants = []
    row = result.fetchone()
    while row is not None:
      plant = Plant(
        plantId=row[DEVICES.DEVICE_ID], 
        plantName=row[DEVICES.PLANT_NAME], 
        plantType=row[DEVICES.PLANT_TYPE]
      )
      plants.append(plant)
      row = result.fetchone()

    return (plants, None)
  
  def get_plant_sensor_data_logs(self, plantId) -> (List[SensorDataLog], str):
    """Return a list of the sensor data longs for the plant (deviceId = plantId)
    Returns: (list_of_sensor_data_logs, None) on success, (None, errorMessage) on failure"""
    deviceId = plantId

    # Verify deviceId exists
    if not self.__exists(DEVICES.TABLE_NAME, DEVICES.DEVICE_ID, deviceId):
      return (None, "deviceId does not exist in devices table")
    
    # Get device's sensor data logs
    result = self.__get_sensor_data_logs(deviceId)

    # Format Data
    sensor_logs = []
    row = result.fetchone()
    while row is not None:
      sensorLog = SensorDataLog(
        timestamp=row[LOGS.TIMESTAMP], 
        soilMoisture=row[LOGS.SOIL_MOISTURE], 
        sunlight=row[LOGS.SUNLIGHT]
      )
      sensor_logs.append(sensorLog)
      row = result.fetchone()

    return (sensor_logs, None)
  
  def get_plant_water_history(self, plantId) -> List[str]:
    """Return a list of the timestamps for when the given plant was watered.
    Returns: (list_of_timestamps, None) on success, (None, errorMessage) on failure"""
    deviceId = plantId

    # Verify deviceId exists
    if not self.__exists(DEVICES.TABLE_NAME, DEVICES.DEVICE_ID, deviceId):
      return (None, "deviceId does not exist in devices table")
    
    # Get device's sensor data logs
    result = self.__get_water_history(deviceId)

    # Format Data
    water_history = []
    row = result.fetchone()
    while row is not None:
      timestamp = row[WATER_LOGS.TIMESTAMP]
      water_history.append(timestamp)
      row = result.fetchone()

    return (water_history, None)

  # -- DEVICE <-> SERVER --
  def device_init(self, data: DeviceInit) -> str:
    """Initialize a new device with the given information, adding it's information to
    the database, & return the unique ID created for the device.
    Returns: deviceId on success, None on failure"""

    # Verify userId exists
    if not self.__exists(USERS.TABLE_NAME, USERS.USER_ID, data.userId):
      print("DeviceInit Error: userId does not exist in users table")
      return None
    
    # Add new entry to Devices Table
    deviceId = self.__generate_uniq_device_id()
    self.__insert_devices(deviceId, data.userId, data.plantName, data.plantType, data.plantSize)
    return deviceId
  
  def device_check_in(self, data: DeviceCheckIn) -> None:
    """Log soil moisture and sunlight information to the database for the device"""

    # Verify device Id exists
    if not self.__exists(DEVICES.TABLE_NAME, DEVICES.DEVICE_ID, data.deviceId):
      print("DeviceCheckIn Error: deviceId does not exists in devices table")
      return
    
    # Add entry to deviceLogs table
    self.__insert_device_logs(data.deviceId, data.soilMoisture, data.sunlight)
    return
  
  def device_water_confirm(self, data: DeviceCredentials) -> None:
    """Log timestamp to waterLogs for the given device"""
    
    # Verify device Id exists
    if not self.__exists(DEVICES.TABLE_NAME, DEVICES.DEVICE_ID, data.deviceId):
      print("DeviceWaterConfirm Error: deviceId does not exists in devices table")
      return
    
    # Add entry to waterLogs table
    self.__insert_water_logs(data.deviceId)
    return
    



  # -- Helper Methods -- 
  def __generate_uniq_user_id(self):
    """generates a unique user id"""
    GENERATION_LENGTH = 25
    userId = generate_id(GENERATION_LENGTH)
    while self.__exists(USERS.TABLE_NAME, USERS.USER_ID, userId):
      userId = generate_id(GENERATION_LENGTH)
    return userId  

  def __generate_uniq_device_id(self):
    """generates a unique device id"""
    GENERATION_LENGTH = 25
    deviceId = generate_id(GENERATION_LENGTH)
    while self.__exists(DEVICES.TABLE_NAME, DEVICES.DEVICE_ID, deviceId):
      deviceId = generate_id(GENERATION_LENGTH)
    return deviceId

  def __exists(self, tableName, columnName, value):
    """returns true if the value exists in the given table's column"""
    params = (value, )
    result = self.cursor.execute(f"SELECT * FROM {tableName} WHERE {columnName} = ?", params)
    if result.fetchone() is not None:
      return True
    return False
  
  def __insert_users(self, userId, username, password):
    """insert new row into users table"""
    params = (userId, username, password)
    self.cursor.execute(f"INSERT INTO {USERS.TABLE_NAME} VALUES (?, ?, ?)", params)
    self.connection.commit()

  def __insert_devices(self, deviceId, userId, plantName, plantType, plantSize):
    """insert new row into devices table"""
    params = (deviceId, userId, plantName, plantType, plantSize)
    self.cursor.execute(f"INSERT INTO {DEVICES.TABLE_NAME} VALUES (?, ?, ?, ?, ?)", params)
    self.connection.commit()

  def __insert_device_logs(self, deviceId, soilMoisture, sunlight):
    """insert new row into deviceLogs table"""
    params = (deviceId, soilMoisture, sunlight)
    self.cursor.execute(f"INSERT INTO {LOGS.TABLE_NAME} ({LOGS.DEVICE_ID}, {LOGS.SOIL_MOISTURE}, {LOGS.SUNLIGHT}) VALUES (?, ?, ?)", params)
    self.connection.commit()

  def __insert_water_logs(self, deviceId):
    """insert new row into waterLogs table"""
    params = (deviceId,)
    self.cursor.execute(f"INSERT INTO {WATER_LOGS.TABLE_NAME} ({WATER_LOGS.DEVICE_ID}) VALUES (?)", params)
    self.connection.commit()

  def __get_user_plants(self, userId):
    """returns the plants for a user"""
    params = (userId,)
    result = self.cursor.execute(f"SELECT {DEVICES.DEVICE_ID}, {DEVICES.PLANT_NAME}, {DEVICES.PLANT_TYPE} FROM {DEVICES.TABLE_NAME} WHERE {DEVICES.USER_ID} = ?", params)
    return result 
  
  def __get_sensor_data_logs(self, deviceId):
    """returns the sensor data logs for a device"""
    params = (deviceId,)
    result = self.cursor.execute(f"SELECT {LOGS.TIMESTAMP}, {LOGS.SOIL_MOISTURE}, {LOGS.SUNLIGHT} FROM {LOGS.TABLE_NAME} WHERE {LOGS.DEVICE_ID} = ?", params)
    return result 
  
  def __get_water_history(self, deviceId):
    """returns the water log timestamps for a device"""
    params = (deviceId,)
    result = self.cursor.execute(f"SELECT {WATER_LOGS.TIMESTAMP} FROM {WATER_LOGS.TABLE_NAME} WHERE {WATER_LOGS.DEVICE_ID} = ?", params)
    return result 

 # -- Other Helper Methods
def generate_id(length):
  letters = string.ascii_lowercase + string.ascii_uppercase + string.digits
  return ''.join(random.choice(letters) for i in range(length))
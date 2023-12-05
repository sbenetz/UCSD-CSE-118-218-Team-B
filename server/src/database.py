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
    return (None, None)


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
    self.__insert_devices(deviceId, data.userId, data.plantName, data.plantType)
    return deviceId

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

  def __insert_devices(self, deviceId, userId, plantName, plantType):
    """insert new row into devices table"""
    params = (deviceId, userId, plantName, plantType)
    self.cursor.execute(f"INSERT INTO {DEVICES.TABLE_NAME} VALUES (?, ?, ?, ?)", params)
    self.connection.commit()

 # -- Other Helper Methods
def generate_id(length):
  letters = string.ascii_lowercase + string.ascii_uppercase + string.digits
  return ''.join(random.choice(letters) for i in range(length))
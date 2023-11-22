# class for interacting with sqlite3 database
import sqlite3 # for connecting to SQLite Database
import random, string # for random userId generation

from common import *

# -- Database --
# users table
USERS_TABLE = 'users'
USERS_USER_ID = 'userId'
USERS_USERNAME = 'username'
USERS_PASSWORD = 'password'

DEVICES_TABLE = 'devices'


class Database:
  def __init__(self, path_to_db):
    print(f"connecting to database at '{path_to_db}'")
    self.connection = sqlite3.connect(path_to_db)
    self.cursor = self.connection.cursor()

  # Return a JSON object of users
  def get_users(self):
    result = self.cursor.execute("select * from users")
    return { "users": result.fetchall() }
  
  # Return a JSON object of devices
  def get_devices(self):
    result = self.cursor.execute("SELECT * from devices")
    return { "devices": result.fetchall() }
  
  # -- Public Methods --

  # Create account given the current data
  # returns (userId, errorMessage)
  def create_account(self, data: Account):

    # validate username & password
    # TODO: Cleanse username and password
    if not data.username: return (None, "invalid username")
    if not data.password: return (None, "invalid password")

    # username must be unique
    if self.__exists(USERS_TABLE, USERS_USERNAME, data.username): 
      return (None, "username already exists")

    # Add new entry to Users
    userId = self.__generate_uniq_user_id()
    self.__insert_users(userId, data.username, data.password)
    return (userId, None)

  # def login(self, data: Account):
    # cleanse/validate username & password



  # -- Helper Methods -- 
  def __generate_uniq_user_id(self):
    """generates a unique user id"""
    GENERATION_LENGTH = 25
    userId = generate_id(GENERATION_LENGTH)
    print(f'userId: {userId}')
    while self.__exists(USERS_TABLE, USERS_USER_ID, userId):
      userId = generate_id(GENERATION_LENGTH)
      print(f'remade userId: {userId}')
    return userId    

  def __exists(self, tableName, columnName, value):
    """returns true if the value exists in the given table's column"""
    result = self.cursor.execute(f"SELECT * FROM {tableName} WHERE {columnName}='{value}'")
    if result.fetchone() is not None:
      return True
    return False
  
  def __insert_users(self, userId, username, password):
    """insert new row into users table"""
    self.cursor.execute(f"INSERT INTO {USERS_TABLE} VALUES ('{userId}', '{username}', '{password}')")
    self.connection.commit()

 # -- Other Helper Methods
def generate_id(length):
  letters = string.ascii_lowercase + string.ascii_uppercase + string.digits
  return ''.join(random.choice(letters) for i in range(length))
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
DEVICES_TABLE = 'devices'


class Database:
  # -- Public Methods --

  def __init__(self, path_to_db):
    """Constructor"""
    print(f"connecting to database at '{path_to_db}'")
    self.connection = sqlite3.connect(path_to_db)
    self.connection.row_factory = sqlite3.Row
    self.cursor = self.connection.cursor()

  def create_account(self, data: Credentials):
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

  def login(self, data: Credentials):
    """Return the userId of the user with the given credentials.
    Returns: (userId, None) on success, (None, errorMessage) on failure"""
    # TODO: Cleanse username and password
    if not data.username: return (None, "invalid username")
    if not data.password: return (None, "invalid password")

    # get user by username
    result = self.cursor.execute(f"SELECT * FROM {USERS.TABLE_NAME} WHERE {USERS.USERNAME}='{data.username}'")
    user = result.fetchone()
    if user is None:
      return (None, "username not found")
    
    # validate password
    if not user[USERS.PASSWORD] == data.password:
      return (None, "wrong password")
    
    # success
    return (user[USERS.USER_ID], None)
    



  # -- Helper Methods -- 
  def __generate_uniq_user_id(self):
    """generates a unique user id"""
    GENERATION_LENGTH = 25
    userId = generate_id(GENERATION_LENGTH)
    print(f'userId: {userId}')
    while self.__exists(USERS.TABLE_NAME, USERS.USER_ID, userId):
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
    self.cursor.execute(f"INSERT INTO {USERS.TABLE_NAME} VALUES ('{userId}', '{username}', '{password}')")
    self.connection.commit()

 # -- Other Helper Methods
def generate_id(length):
  letters = string.ascii_lowercase + string.ascii_uppercase + string.digits
  return ''.join(random.choice(letters) for i in range(length))
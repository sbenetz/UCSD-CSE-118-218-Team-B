# class for interacting with sqlite3 database
import sqlite3

class Database:
  def __init__(self, path_to_db):
    print(f"connecting to database at '{path_to_db}'")
    connection = sqlite3.connect(path_to_db)
    self.cursor = connection.cursor()

  # Return a JSON object of users
  def get_users(self):
    result = self.cursor.execute("select * from users")
    return { "users": result.fetchall() }
  
  # Return a JSON object of devices
  def get_devices(self):
    result = self.cursor.execute("select * from devices")
    return { "devices": result.fetchall() }
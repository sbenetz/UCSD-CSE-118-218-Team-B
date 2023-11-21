# class for interacting with sqlite3 database
import sqlite3

class Database:
  def __init__(self, path_to_db):
    print(f"connecting to database at '{path_to_db}'")
    connection = sqlite3.connect(path_to_db)
    self.cursor = connection.cursor()

  def get(self):
    result = self.cursor.execute("select * from users")
    print('result:', result.fetchall())
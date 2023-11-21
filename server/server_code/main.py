import uvicorn # server for running FastAPI
from fastapi import FastAPI

import socket # for getting this server's IP address

# Create FastAPI instance, this is referenced by uvicorn
app = FastAPI()

@app.get("/")
async def root():
  return {"info": "Smart Plant Water Server"}


# run the app
if __name__ == '__main__':
  # Get my IP Address
  hostname = socket.gethostname()
  ip_address = socket.gethostbyname(hostname)
  print(ip_address)
  # Run the app (first argument is "<name of this file>:<name of FastAPI instance>")
  uvicorn.run("main:app", host=ip_address, port=8000, reload=True)
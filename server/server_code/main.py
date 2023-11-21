import uvicorn # server for running FastAPI
from fastapi import FastAPI

import sys # for getting command line arguments

EXPECTED_USAGE = "USAGE: python3 main.py <ip_address_to_run_server_on>"

# Create FastAPI instance, this is referenced by uvicorn
app = FastAPI()

@app.get("/")
async def root():
  return {"info": "Smart Plant Water Server"}


# run the app
if __name__ == '__main__':
  # Get my IP Address from sys.args
  if (len(sys.argv) < 2 or sys.argv[1] == ""):
    print(EXPECTED_USAGE)
    sys.exit(1)
  ip_address = sys.argv[1]
  # Run the app (first argument is "<name of this file>:<name of FastAPI instance>")
  uvicorn.run("main:app", host=ip_address, port=8000, reload=True)
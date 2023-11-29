# Main File: API Server
# USAGE: uvicorn main:app --reload --host <ip_address> --port <port_num>
from fastapi import FastAPI


# Create FastAPI instance, this is referenced by uvicorn
app = FastAPI()

# Define API Rules
@app.get("/")
async def root():
  return {"info": "Smart Plant Water Server"}


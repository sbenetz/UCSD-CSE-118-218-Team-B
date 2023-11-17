from fastapi import FastAPI

# Creat FastAPI instance, this is referenced by uvicorn
app = FastAPI()

@app.get("/")
async def root():
  return {"info": "Smart Plant Water Server"}

# to run: uvicorn main:app --reload
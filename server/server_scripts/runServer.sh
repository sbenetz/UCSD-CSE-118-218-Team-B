#!/bin/bash
# Starts up the FastAPI server
# NOTE: must be run with 'sudo'
DIR_NAME=server_code

source env/bin/activate
cd ${DIR_NAME}
echo $pwd
uvicorn main:app --reload
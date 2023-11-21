#!/bin/bash
# Starts up the FastAPI server
# NOTE: must be run with 'sudo'
DIR_NAME=server_code
IP_ADDRESS=$(hostname -I | cut -d ' ' -f1)
PORT_NUMBER=8000

source env/bin/activate
cd ${DIR_NAME}
uvicorn main:app --reload --host ${IP_ADDRESS} --port ${PORT_NUMBER}
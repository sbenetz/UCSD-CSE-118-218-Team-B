#!/bin/bash
# Starts up the FastAPI server
# NOTE: must be run with 'sudo'
DIR_NAME=server_code
IP_ADDRESS=$(hostname -I | cut -d ' ' -f1)

source env/bin/activate
cd ${DIR_NAME}
echo ${IP_ADDRESS}
python3 main.py ${IP_ADDRESS}
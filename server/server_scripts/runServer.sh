#!/bin/bash
# Starts up the FastAPI server
# NOTE: must be run with 'sudo'
DIR_NAME=src
PORT_NUMBER=8000
LOG_FILE=output.log

# Kill current process using PORT_NUMBER if exists
CURR_PROC_PID=$(lsof -i :8000 | awk 'NR==2 {print $2}')
if [ -n "${CURR_PROC_PID}" ]; then
  echo "Killing process ${CURR_PROC_PID}"
  kill ${CURR_PROC_PID}
fi

# Activate Python environment
source env/bin/activate

# Start the server
cd ${DIR_NAME}
nohup uvicorn main:app --port ${PORT_NUMBER} > ${LOG_FILE} 2>&1 &
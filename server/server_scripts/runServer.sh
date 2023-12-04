#!/bin/bash
# Run the server
SRC_DIR=src
LOG_FILE=output.log 
API_SERVER_PORT=8000

# -- Run Server --
echo "Running the server"

# Kill any process currently using API_SERVER_PORT if exists
CURR_PROC_PID=$(lsof -i :${API_SERVER_PORT} | awk 'NR==2 {print $2}')
if [ -n "${CURR_PROC_PID}" ]; then
  echo "Port ${API_SERVER_PORT} in use. Killing process ${CURR_PROC_PID}"
  kill ${CURR_PROC_PID}
fi

# Activate Python environment
source env/bin/activate

# Start the server
cd ${SRC_DIR}
nohup uvicorn main:app --port ${API_SERVER_PORT} > ${LOG_FILE} 2>&1 &

# Monitor Server Output
tail -f ${LOG_FILE}

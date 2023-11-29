#!/bin/bash
# USAGE: port_number is the port to run the server on
USAGE="Usage: $0 <port_number>"

SRC_DIR=src
LOG_FILE=output.log 

# -- Parse Cmd Line Arguments --
if [ "$#" -ne 1 ]; then
    echo USAGE
    exit 1
fi
API_SERVER_PORT=$1


# -- Update Server Source Code --
echo "-- Update API Server On Port ${API_SERVER_PORT} --"

# Kill any process currently using API_SERVER_PORT if exists
CURR_PROC_PID=$(lsof -i :${API_SERVER_PORT} | awk 'NR==2 {print $2}')
if [ -n "${CURR_PROC_PID}" ]; then
  echo "Port ${API_SERVER_PORT} in use. Killing process ${CURR_PROC_PID}"
  kill ${CURR_PROC_PID}
fi

# Update SRC_DIR
echo "Replacing '${SRC_DIR}' directory"
rm -rf ${SRC_DIR}
unzip ${SRC_DIR}.zip
rm ${SRC_DIR}.zip

# -- Running Server --
echo "Running the server"
# Activate Python environment
source env/bin/activate

# Start the server
cd ${SRC_DIR}
nohup uvicorn main:app --port ${API_SERVER_PORT} > ${LOG_FILE} 2>&1 &

# Monitor Server Output
tail -f ${LOG_FILE}




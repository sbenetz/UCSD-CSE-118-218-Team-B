#!/bin/bash
# Build script

SRC_DIR=src
UPDATE_SCRIPT_NAME=updateServer.sh
UPDATE_SCRIPT_PATH=server_scripts/${UPDATE_SCRIPT_NAME}
RUN_SCRIPT_NAME=runServer.sh
RUN_SCRIPT_PATH=server_scripts/${RUN_SCRIPT_NAME}
USERNAME=root
HOSTNAME="8.tcp.us-cal-1.ngrok.io"
HOST_PORT=12976

API_SERVER_DIR=server
API_SERVER_PORT=8000

echo "Building to Raspberry Pi"

# Zip up src directory
echo "cmd: zip -r ${SRC_DIR}.zip ${SRC_DIR}"
zip -r ${SRC_DIR}.zip ${SRC_DIR}

# Send zipped src directory & the update script to remote
#CMD1="scp -P ${HOST_PORT} ${UPDATE_SCRIPT_PATH} ${SRC_DIR}.zip ${USERNAME}@${HOSTNAME}:${API_SERVER_DIR}"
CMD1="scp ${SRC_DIR}.zip ${UPDATE_SCRIPT_PATH} ${RUN_SCRIPT_PATH} root@raspberrypiteamb.local:${API_SERVER_DIR}/"
echo "cmd: $CMD1"
eval "$CMD1"

# Run the update script on the remote
#CMD2="ssh -p ${HOST_PORT} ${USERNAME}@${HOSTNAME}:${API_SERVER_DIR}/ 'cd ${API_SERVER_DIR}; ./${UPDATE_SCRIPT_NAME}'"
CMD2="ssh root@raspberrypiteamb.local 'cd ${API_SERVER_DIR}; ./${UPDATE_SCRIPT_NAME} ${API_SERVER_PORT};'"
echo "cmd: $CMD2"
eval "$CMD2"


# cleanup
rm ${SRC_DIR}.zip



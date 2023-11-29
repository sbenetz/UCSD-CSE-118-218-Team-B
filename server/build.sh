#!/bin/bash
SRC_DIR=src
UPDATE_SCRIPT_NAME=updateServer.sh
UPDATE_SCRIPT_PATH=server_scripts/${UPDATE_SCRIPT_NAME}
USERNAME=root
HOSTNAME="8.tcp.us-cal-1.ngrok.io"
HOST_PORT=12976
OUTPUT_DIR=prod-server

echo "Building to production"

# Zip up src directory
echo "cmd: zip -r ${SRC_DIR}.zip ${SRC_DIR}"
zip -r ${SRC_DIR}.zip ${SRC_DIR}

# Send zipped src directory & the update script to remote
echo "cmd: scp -P ${HOST_PORT} ${UPDATE_SCRIPT_PATH} ${SRC_DIR}.zip ${USERNAME}@${HOSTNAME}:${OUTPUT_DIR}/"
scp -P ${HOST_PORT} ${UPDATE_SCRIPT_PATH} ${SRC_DIR}.zip ${USERNAME}@${HOSTNAME}:${OUTPUT_DIR}/

# Run the update script on the remote
echo "cmd: ssh -p ${HOST_PORT} ${USERNAME}@${HOSTNAME}:${OUTPUT_DIR}/ 'cd ${OUTPUT_DIR}; ./${UPDATE_SCRIPT_NAME}'"
ssh -p ${HOST_PORT} ${USERNAME}@${HOSTNAME} "cd ${OUTPUT_DIR}; ./${UPDATE_SCRIPT_NAME}"

# cleanup
rm ${SRC_DIR}.zip



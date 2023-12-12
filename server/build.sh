#!/bin/bash
# Build script
# This script copies the src/ directory & runServer.sh and updateServer.sh from server_scripts/ to the Raspberry Pi server, then it runs the updateServer.sh script on that server so that the server is updated with the new code and the API server service itself is restarted on the RaspberryPi

SRC_DIR=src
UPDATE_SCRIPT_NAME=updateServer.sh
UPDATE_SCRIPT_PATH=server_scripts/${UPDATE_SCRIPT_NAME}
RUN_SCRIPT_NAME=runServer.sh
RUN_SCRIPT_PATH=server_scripts/${RUN_SCRIPT_NAME}
USERNAME=root

API_SERVER_DIR=server
API_SERVER_PORT=8000

# Check if both host and port arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <host> <port>"
    exit 1
fi

# Assign command-line arguments to variables
HOST="$1"
PORT="$2"

echo "Building to Raspberry Pi"

# Zip up src directory
echo "cmd: zip -r ${SRC_DIR}.zip ${SRC_DIR}"
zip -r ${SRC_DIR}.zip ${SRC_DIR}

# Send zipped src directory & the update script to remote
CMD1="scp -P ${PORT} ${SRC_DIR}.zip ${UPDATE_SCRIPT_PATH} ${RUN_SCRIPT_PATH} ${USERNAME}@${HOST}:${API_SERVER_DIR}"
echo "cmd: $CMD1"
eval "$CMD1"

# Run the update script on the remote
CMD2="ssh -p ${PORT} ${USERNAME}@${HOST} 'cd ${API_SERVER_DIR}; ./${UPDATE_SCRIPT_NAME} ${API_SERVER_PORT};'"
echo "cmd: $CMD2"
eval "$CMD2"


# cleanup
rm ${SRC_DIR}.zip



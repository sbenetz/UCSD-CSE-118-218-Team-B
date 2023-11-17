#!/bin/bash
# NOTE: script to be run on the server which will update & run the server after a build was performed & pushed onto the server
# NOTE: must be run with 'sudo'
DIR_NAME=server_code

mv ../${DIR_NAME}.zip ${DIR_NAME}.zip
unzip ${DIR_NAME}.zip
rm ${DIR_NAME}.zip
./runServer.sh
#!/bin/bash
DIR_NAME=server_code
USERNAME=cse118
HOSTNAME=raspberrypi.local

zip -r ${DIR_NAME}.zip ${DIR_NAME}
scp ${DIR_NAME}.zip ${USERNAME}@${HOSTNAME}:
rm ${DIR_NAME}.zip



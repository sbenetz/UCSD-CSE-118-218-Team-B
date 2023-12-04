#!/bin/bash
# Performs a build that updates the production API server running
# on the Raspberry Pi
API_SERVER_DIR=prod-server
API_SERVER_PORT=8000
./__build.sh ${API_SERVER_DIR} ${API_SERVER_PORT}
#!/bin/bash
# Performs a build that updates the development API server running
# on the Raspberry Pi
API_SERVER_DIR=dev-server
API_SERVER_PORT=8001
./__build.sh ${API_SERVER_DIR} ${API_SERVER_PORT}
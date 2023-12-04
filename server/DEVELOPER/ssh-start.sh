#!/bin/bash
# Connects via ssh to the Raspberry Pi Server
# NOTE: URL must be updated every time the ngrok service on the raspberry pi restarts (see dashboard.ngrok.com for tcp url)
URL=tcp://2.tcp.ngrok.io:13300

# Extract host and port
HOST_PORT=$(echo "$URL" | awk -F '://' '{print $2}')
HOST=$(echo "$HOST_PORT" | awk -F ':' '{print $1}')
PORT=$(echo "$HOST_PORT" | awk -F ':' '{print $2}')

# Run the SSH command
ssh root@$HOST -p $PORT
# Raspberry PI 4 Server
This directory stores the code for the Raspberry PI server in the "Smart Plant Water" project

## Startup API Server on Raspberry Pi
### SSH into Raspberryp Pi
NOTE: The Raspberry Pi uses public/private key authentication for SSH access. Thus SSH will only work if you have the correct private key.

**If on the same network**
```sh
ssh root@raspberrypiteamb.local
```

**If accessing remotely**
```sh
ssh root@<PUBLIC-IP-ADDR-OF-JACKSONS-HOME-ROUTER> -p <THE-EXTERNAL-PORT-USED>
```
Note: talk to Jackson for the above information.

### Ensure Ngrok is running
Ensure Ngrok is running:
```sh
ps aux | grep ngrok
```
If it is not, then see DEVELOPER/DEVELOPER_NOTES.md

### Run the API Server
```sh
cd /root/prod-server
./runServer
```
Check that the server log:
```sh
cat src/output.log
```
Ensure the server is listening on desired port:
```sh
lsof -i :<port_number>
```


## Build & Run From Development Environment
To 'build' means basically just to copy the code from the developer's local machine onto the Raspberry Pi. This is useful so that the developer can develop in their local environment and then push the changes to the Raspberry Pi.

To build & run for the **development** API server:
```sh
./dev-build.sh
```

To build & run for the **production** API server:
```sh
./prod-build.sh
```

## Access API Servers
### Production
Endpoint: https://fit-glowworm-promptly.ngrok-free.app
### Development 
Endpoint (subject to change): https://2ea4-70-95-163-60.ngrok-free.app
### API Documentation
For each endpoint, the endpoint's API documentation is accessible with `<endpoint>/redoc`
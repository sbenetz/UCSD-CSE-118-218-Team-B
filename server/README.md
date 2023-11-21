# Raspberry PI 4 Server
This directory stores the code for the Raspberry PI server in the "Smart Plant Water" project

## File Organization
The [server_code/](server_code/) directory stores the code that will actually be run on the server

The [server_scripts/](server_scripts/) directory stores scripts for the server. These are copy and pasted over to the server to be run (not included in the build), since they have to do with updating and running the server after a new build has come in.

## Build & Run Process
### Build
To 'build' means basically just to copy the code from the developer's local machine onto the Raspberry Pi. This is useful so that the developer can develop in their local environment and then push the changes to the Raspberry Pi.

To build, simply run the following:
```sh
./build.sh
```
### Updating & Running the Server
1. SSH into the Raspberry Pi
2. Enter into root privaleges:
```sh
sudo su -
```
3. Navigate to the server directory:
```sh
cd /home/cse118/server
```
4. Run the update script (this will also start the server):
```sh
./updateServer.sh
```
NOTE: if the server's code is already updated, the server can also be started with the run script in the server directory:
```sh
./runServer.sh
```




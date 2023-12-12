# DEVELOPER NOTES
Notes for Developers

## SSH Setup For Raspberry Pi (local network)
NOTE: this is information about SSHing into a Rasbperry Pi.

REQUIREMENT: Raspberry Pi must be on the same network as your machine. Note, if needed you can run a ethernet connection from your computer to the Raspberry Pi & use that connection to SSH into the Raspberry Pi, using the Raspberry Pi's host name (default is `raspberrypi.local`). Then you can connect the Raspberry Pi to your local WiFi network, remove the ethernet cables, and simply SSH over wifi after that.

Our Pi's SSH: `ssh cse118@raspberrypiteamb.local` (I changed the hostname to `raspberrypiteamb.local`)

Notes for setting up SSH key authentication. What I did:
- Added my computer's public key to the root & cse118 users so that I can ssh into both of them without a password
  - Used `ssh-copy-id cse118@raspberrypiteamb.local`
  - Copied authorized_keys file from cse118 user to root directory as well
- set `PasswordAuthentication no` in `/etc/ssh/sshd_config` in order to disable ssh login via password

### Working with SSH
Check sshd status:
```sh
sudo systemctl status ssh
```
View ssh logs:
```sh
journalctl -fu ssh
```

## Ngrok on Raspberry Pi
### Setup
Install Ngrok:
```sh
curl -s https://ngrok-agent.s3.amazonaws.com/ngrok.asc | sudo tee /etc/apt/trusted.gpg.d/ngrok.asc >/dev/null && echo "deb https://ngrok-agent.s3.amazonaws.com buster main" | sudo tee /etc/apt/sources.list.d/ngrok.list && sudo apt update && sudo apt install ngrok
```
Add your authtoken to the default ngrok.yml configuration file:
```sh
ngrok config add-authtoken <TOKEN>
```
Add logging & tunnels configuration to Ngrok config file (`/root/.config/ngrok/ngork.yml`). Example:
```yml
version: "2"
authtoken: <YOUR-AUTH-TOKEN>
log_level: info
log: /root/.config/ngrok/ngrok.log

tunnels:
  prod-api:
    addr: 8000
    proto: http
    inspect: false
    schemes:
      - http
      - https
    domain: <YOUR-DOMAIN-NAME>
```

### Setup Ngrok Service to start on startup
**Create script that starts the ngrok service**
```sh
nano /root/.config/ngrok/start-ngrok.sh
```
Copy and paste the below into the file (starts Ngrok service to run continuall in the background):
```sh
#!/bin/bash
nohup ngrok start --all --config="/root/.config/ngrok/ngrok.yml" & > nohup.out 2>&1
```
Add permission to execute to the script:
```sh
chmod +x /root/.config/ngrok/start-ngrok.sh
```
**Setup the above script to run at reboot**
```sh
nano /etc/rc.local
```
Add the following line & save:
```sh
/root/.config/ngrok/start-ngrok.sh
```
Reboot the PI

Check https://dashboard.ngrok.com/cloud-edge/endpoints for the URLs to access your endpoints

**Ex: Connect to Production API Sever**
`ngrok.log` output:
```log
t=2023-11-28T21:31:55-0800 lvl=info msg="started tunnel" obj=tunnels name=prod-api addr=http://localhost:8000 url=https://2638-2603-8001-8d00-7e27-00-19c9.ngrok-free.app
```
Then Endpoint: `https://2638-2603-8001-8d00-7e27-00-19c9.ngrok-free.app`

**Ex: Connect to Development API Sever**
`ngrok.log` output:
```log
t=2023-11-28T21:31:55-0800 lvl=info msg="started tunnel" obj=tunnels name=test-api addr=http://localhost:8001 url=https://1392-2603-8001-8d00-7e27-00-19c9.ngrok-free.app
```
Then Endpoint: `https://1392-2603-8001-8d00-7e27-00-19c9.ngrok-free.app`




## Setup New API Server On Raspberry Pi
Create new directory:
```sh
mkdir <directory-name>
cd <directory-name>
```
### Setup Python Virtual Environment (venv)
Create venv:
```sh
python3 -m venv env
```
Install necessary packages:
```sh
source env/bin/activate
pip install uvicorn
pip install fastapi
```
(`deactivate` exits the virtual environment)

### Setup Database
```sh
mkdir database
cd database
nano schema.sql
```
Copy and paste the contents of [database_schema/schema.sql](database_schema/schema.sql) into the nano text editor & save.

Create database from schema:
```sh
sqlite3 database.db < schema.sql
```

### Running
Now, from your local machine, run the build script from the server repository:
```sh
./build.sh
```


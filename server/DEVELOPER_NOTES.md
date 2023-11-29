# DEVELOPER NOTES
These can be ignored
## SSH Setup (local network)
REQUIREMENT: Raspberry Pi must be on the same network as your machine

Default SSH: `ssh cse118@raspberrypiteamb.local`

Changes:
- Added my computer's public key to the root & cse118 users so that I can ssh into both of them without a password
  - Used `ssh-copy-id cse118@raspberrypiteamb.local`
  - Copied authorized_keys file from cse118 user to root directory as well
- set `PasswordAuthentication no` in `/etc/ssh/sshd_config` in order to disable ssh login via password

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
Add logging & tunnels configuration to Ngrok config file (`~/.config/ngrok/ngork.yml`):
```yml
version: "2"
authtoken: <YOUR-AUTH-TOKEN>

log_level: info
log: /root/.config/ngrok/ngrok.log

tunnels:
  ssh-server:
    proto: tcp
    addr: 22
  test-api:
    proto: http
    addr: 8001
  prod-api:
    proto: http
    addr: 8000
```

### Start Ngrok Service
Start Ngrok service to run continually with the above tunnels:
```sh
nohup ngrok start --all --config="/root/.config/ngrok/ngrok.yml" &
```
Note: after running, 'ctrl+c' in order to allow further commands to be entered into terminal.

Check log file for the URLs to access the tunnels:
```sh
cat ~/.config/ngrok/ngrok.log
```

**Ex: Connect to SSH Server**

`ngrok.log` output:
```log
t=2023-11-28T21:31:55-0800 lvl=info msg="started tunnel" obj=tunnels name=ssh-server addr=//localhost:22 url=tcp://8.tcp.us-cal-1.ngrok.io:12976
```
Then Client SSH Command:
```sh
ssh root@8.tcp.us-cal-1.ngrok.io -p 12976
```

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




## Setup API Server
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
```sh
lsof -i :8000
```


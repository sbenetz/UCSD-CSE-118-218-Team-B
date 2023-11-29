# Raspberry PI 4 Server
This directory stores the code for the Raspberry PI server in the "Smart Plant Water" project

## Build & Run Process
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
These are subject to change if the Ngrok service on the Raspberry Pi gets restarted
### Production
Endpoint: https://2638-2603-8001-8d00-7e27-00-19c9.ngrok-free.app
### Development
Endpoint: https://1392-2603-8001-8d00-7e27-00-19c9.ngrok-free.app
### API Documentation
For each endpoint, the endpoint's API documentation is accessible with `<endpoint>/redoc`
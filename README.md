# Walkie-Talkie App

A real-time communication application allowing users to host or join encrypted chat servers using Windows-exclusive networking capabilities provided by WS2tcpip.

## Description

This program enables users to either host or join an end-to-end encrypted chat server. It is specifically designed for the Windows operating system, utilizing the WS2tcpip library which is tailored for managing sockets and internet connections on Windows.

The application supports creating user accounts, verifying users, and allows communication over any available ports.

### Dependencies

- Windows OS
- Microsoft Visual Studio or a compatible C++ compiler that supports C++11 or later
- The program uses Winsock2 API, specific to Windows, for network communication.

### Installing

- Download the source code from the repository or clone it using Git.
- Open the solution in Microsoft Visual Studio.

### Executing program

- Compile the program using Visual Studio.
- When running the executable, You will be prompted to either create a user or log in if you already have an account.
- Once authenticated, you can choose to host a server or connect to an existing one by providing the IP address and port number.
- To host a server, specify the port you want the server to listen on.
- If the user is the first to connect to the server they must choose the encryption key
- To join a server, specify the server's IP address and the port it is listening on, along with the encryption key for secure communication.

```bash
# comand to compile code
g++ '.\mainapp.cpp' '.\server.cpp' '.\client.cpp' '.\CSV.cpp' -o main.exe -lws2_32
```
```bash
App.exe
```

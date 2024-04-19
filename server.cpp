#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    int client;
    int portNum = 1500;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];

    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    if (WSAStartup(ver, &wsData) != 0) {
        cerr << "Error initializing Winsock." << endl;
        return 1;
    }

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);

    if (server == INVALID_SOCKET) {
        cerr << "Error creating socket." << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portNum);

    if (bind(server, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Error binding connection." << endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    cout << "Looking for clients..." << endl;

    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Error listening on socket." << endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    sockaddr_in client_addr;
    int clientSize = sizeof(client_addr);
    client = accept(server, (sockaddr*)&client_addr, &clientSize);

    if (client == INVALID_SOCKET) {
        cerr << "Error accepting client." << endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    // Timeout if no data
    int timeout = 10000; 
    setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    cout << "Client connected on port " << ntohs(client_addr.sin_port) << endl;
    // hi messeg 
    send(client, "hi", 2, 0);

    while (!isExit) {
        int bytesReceived = recv(client, buffer, bufsize, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            // Handle timeout or error
            if (WSAGetLastError() == WSAETIMEDOUT) {
                cerr << "Timeout: No data received from client." << endl;
                continue; // Continue waiting for data
            } else {
                cerr << "Error receiving data from client." << endl;
                isExit = true;
            }
        } else {
            buffer[bytesReceived] = '\0'; // Null-terminate the received data
            cout << "Client: " << buffer << endl;
            if (strcmp(buffer, "#") == 0) {
                isExit = true;
            } 
            else {
                cout << "Server: ";
                cin.getline(buffer, bufsize);
                send(client, buffer, strlen(buffer), 0);
                if (strcmp(buffer, "#") == 0) {
                    isExit = true;
                }
            }
        }
    }
    cout << "Connection terminated." << endl;
    closesocket(client);
    closesocket(server);
    WSACleanup();
    return 0;
}

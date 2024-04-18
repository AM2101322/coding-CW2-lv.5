#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    /* ---------- INITIALIZING VARIABLES ---------- */

    int client, server;
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

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/

    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server == INVALID_SOCKET) {
        cerr << "Error creating socket." << endl;
        WSACleanup();
        return 1;
    }

    cout << "\n=> Socket server has been created..." << endl;

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

    cout << "=> Looking for clients..." << endl;

    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Error listening on socket." << endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    /* ------------- ACCEPTING CLIENTS  ------------- */

    sockaddr_in client_addr;
    int clientSize = sizeof(client_addr);
    client = accept(server, (sockaddr*)&client_addr, &clientSize);

    if (client == INVALID_SOCKET) {
        cerr << "Error accepting client." << endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    cout << "=> Client connected on port " << ntohs(client_addr.sin_port) << endl;
    closesocket(server); // Close the listening socket

    /* ------------- COMMUNICATION  ------------- */

    do {
        recv(client, buffer, bufsize, 0);
        cout << "Client: " << buffer << endl;

        if (strcmp(buffer, "#") == 0) {
            isExit = true;
            send(client, buffer, bufsize, 0); // Echo the termination signal back to client
            break;
        }

        cout << "Server: ";
        cin.getline(buffer, bufsize);
        send(client, buffer, bufsize, 0);

        if (strcmp(buffer, "#") == 0) {
            isExit = true;
            break;
        }

    } while (!isExit);

    /* ---------------- CLOSE CONNECTION ------------- */

    cout << "Connection terminated." << endl;
    closesocket(client);
    WSACleanup();
    return 0;
}

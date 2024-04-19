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
    const char* ip = "127.0.0.1";

    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    if (WSAStartup(ver, &wsData) != 0) {
        cerr << "Error initializing Winsock." << endl;
        return 1;
    }

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client == INVALID_SOCKET) {
        cerr << "Error creating socket." << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Error connecting to server." << endl;
        closesocket(client);
        WSACleanup();
        return 1;
    }

    // Timeout if no data
    int timeout = 5000; 
    setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    // Connection established
    cout << "Connection to the server port number: " << portNum << endl;
    cout << "Awaiting confirmation from the server..." << endl;
    cout << "step 1 client";

    int bytesReceived = recv(client, buffer, bufsize, 0);
    cout << "step 2 client";
    if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
        cerr << "Error receiving confirmation from server." << endl;
        closesocket(client);
        WSACleanup();
        return 1;
        cout << "step 3 client";
    } 
    else {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        cout << "=> Connection confirmed, you are good to go..." << endl;
        cout << "\n\n=> Enter # to end the connection\n" << endl;
        cout << "step 4 client";

        while (!isExit) {
            cout << "Client: ";
            cin.getline(buffer, bufsize);

            if (send(client, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
                cerr << "Error sending data to server." << endl;
                closesocket(client);
                WSACleanup();
                return 1;
                cout << "step 5 client";
            }
            if (strcmp(buffer, "#") == 0) {
                isExit = true;
                break;
            }
            bytesReceived = recv(client, buffer, bufsize, 0);
            cout << "step 6 client";
            if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
                // Handle timeout or error
                if (WSAGetLastError() == WSAETIMEDOUT) {
                    cerr << "Timeout: No data received from server." << endl;
                    continue; // Continue waiting for data
                } else {
                    cerr << "Error receiving data from server." << endl;
                    closesocket(client);
                    WSACleanup();
                    return 1;
                    cout << "step 7 client";
                }
            } 
            else {
                cout << "step 8 client";
                buffer[bytesReceived] = '\0'; // Null-terminate the received data
                cout << "Server: " << buffer << endl;

                if (strcmp(buffer, "#") == 0) {
                    isExit = true;
                    break;
                }
            }
        }
    }

    cout << "\n=> Connection terminated.\nGoodbye...\n";

    closesocket(client);
    WSACleanup();
    return 0;
}

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
    // Send "hi" message
    send(client, "hi", 2, 0);

    // Timeout if no data
    int timeout = 10000; 
    setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    // conaction worked 
    cout << "Connection to the server port number: " << portNum << endl;
    cout << "Awaiting confirmation from the server..." << endl;

    int bytesReceived = recv(client, buffer, bufsize, 0);
    if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
        cerr << "Error receiving confirmation from server." << endl;
        closesocket(client);
        WSACleanup();
        return 1;
    } 
    else {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        cout << "=> Connection confirmed, you are good to go..." << endl;
        cout << "\n\n=> Enter # to end the connection\n" << endl;

        while (!isExit) {
            cout << "Client: ";
            cin.getline(buffer, bufsize);

            send(client, buffer, strlen(buffer), 0);
               
            if (strcmp(buffer, "#") == 0) {
                isExit = true;
                break;
            }
            bytesReceived = recv(client, buffer, bufsize, 0);
            if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
                cerr << "Error receiving data from server." << endl;
                closesocket(client);
                WSACleanup();
                return 1;
            } 
            else {
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

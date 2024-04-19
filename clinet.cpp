#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void receiveMessages(SOCKET client) {
    char buffer[1024];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(client, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            cerr << "Error receiving data from server." << endl;
            break;
        } else {
            buffer[bytesReceived] = '\0';
            cout << buffer << endl;
        }
    }
}

int main() {
    int portNum = 1500;
    const char* ip = "127.0.0.1";

    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    if (WSAStartup(ver, &wsData) != 0) {
        cerr << "Error initializing Winsock." << endl;
        return 1;
    }

    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);

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

    // Start receiving thread
    thread receiveThread(receiveMessages, client);

    // Main loop for sending messages
    string message;
    while (true) {
        getline(cin, message);

        if (message == "kill")
            break;

        send(client, message.c_str(), message.size(), 0);
    }

    // Wait for the receiving thread to finish
    receiveThread.join();

    cout << "Connection terminated." << endl;
    closesocket(client);
    WSACleanup();
    return 0;
}
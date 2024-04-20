#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

const char key[] = "123";
int portNum;
char ip[16]; // Assuming IPv4 address, maximum length is 15 characters + null terminator

using namespace std;

// Function to perform XOR encryption and decryption
void XOR(char* buffer, int size) {
    int keyLen = strlen(key);
    for (int i = 0; i < size; ++i) {
        buffer[i] ^= key[i % keyLen];
    }
}

// Function to receive messages from the server
void receiveMessages(SOCKET client) {
    char buffer[1024];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(client, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            cerr << "Server disconnected" << endl;
            break;
        } else {
            // Decrypt received message
            XOR(buffer, bytesReceived);
            buffer[bytesReceived] = '\0';
            cout << buffer << endl;
        }
    }
}

// Function to create and manage the client connection
void createClient(int portNum, const char* ip) {
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    if (WSAStartup(ver, &wsData) != 0) {
        cerr << "Error initializing Winsock." << endl;
        return;
    }

    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);

    if (client == INVALID_SOCKET) {
        cerr << "Error creating socket." << endl;
        WSACleanup();
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Error connecting to server." << endl;
        closesocket(client);
        WSACleanup();
        return;
    }

    cout << "You are connected." << endl << "To disconnect, type 'kill'." << endl;

    // Start receiving messages in a separate thread
    thread receiveThread(receiveMessages, client);

    // Loop for sending messages
    string message;
    while (true) {
        getline(cin, message);

        if (message == "kill") {
            closesocket(client);
            WSACleanup();
            break;
        }

        // Encrypt message before sending
        XOR(&message[0], message.size());
        send(client, message.c_str(), message.size(), 0);
    }
}

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

const char key[] = "123";
int portNum = 1500;
const char* ip = "127.0.0.1";

using namespace std;

// Function to perform XOR encryption and decryption
void XOR(char* buffer, int size) {
    int keyLen = strlen(key);
    for (int i = 0; i < size; ++i) {
        buffer[i] ^= key[i % keyLen];
    }
}

// this funtion is for the threading to use later
void receiveMessages(SOCKET client) {
    char buffer[1024];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(client, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            cerr << "server disconacted" << endl;
            break;
        } else {
            XOR(buffer, bytesReceived);
            buffer[bytesReceived] = '\0';
            cout << buffer << endl;
        }
    }
}

int main() {

    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    if (WSAStartup(ver, &wsData) != 0) {
        cerr << "Error initializing Winsock.";
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
    std::cout << "you have conected" << endl << "to disconect typ 'kill' " << endl;
    // threading starts receving messagess 
    thread receiveThread(receiveMessages, client);

    // loop for sending messages
    string message;
    while (true) {
        getline(cin, message);

        if (message == "kill") {
            closesocket(client);
            WSACleanup();
            break;
        }

        // Encrypt message 
        XOR(&message[0], message.size());
        send(client, message.c_str(), message.size(), 0);
    }

}

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

void receiveMessages(SOCKET client) {
    char buffer[1024];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(client, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            std::cerr << "Error receiving data from client." << std::endl;
            break;
        } else {
            buffer[bytesReceived] = '\0';
            std::cout << "Client: " << buffer << std::endl;
        }
    }
}

int main() {
    int portNum = 1500;

    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    if (WSAStartup(ver, &wsData) != 0) {
        std::cerr << "Error initializing Winsock." << std::endl;
        return 1;
    }

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);

    if (server == INVALID_SOCKET) {
        std::cerr << "Error creating socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portNum);

    if (bind(server, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Error binding connection." << std::endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    std::cout << "Looking for clients..." << std::endl;

    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening on socket." << std::endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    sockaddr_in client_addr;
    int clientSize = sizeof(client_addr);
    SOCKET client = accept(server, (sockaddr*)&client_addr, &clientSize);

    if (client == INVALID_SOCKET) {
        std::cerr << "Error accepting client." << std::endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    // Send "hi" message
    send(client, "hi", 2, 0);

    // Start receiving thread
    std::thread receiveThread(receiveMessages, client);

    // Main loop for sending messages
    std::string message;
    while (true) {
        std::cout << "Server: ";
        std::getline(std::cin, message);

        if (message == "kill")
            break;

        send(client, message.c_str(), message.size(), 0);
    }

    // Wait for the receiving thread to finish
    receiveThread.join();

    std::cout << "Connection terminated." << std::endl;
    closesocket(client);
    closesocket(server);
    WSACleanup();
    return 0;
}

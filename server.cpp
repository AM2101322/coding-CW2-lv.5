#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#include "headers.h"

#pragma comment(lib, "ws2_32.lib")

constexpr int MAX_CLIENTS = 4;
SOCKET clients[MAX_CLIENTS];
int numClients = 0;

void receiveMessages(SOCKET client) {
    char buffer[1024];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(client, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            std::cerr << "Error receiving data from client." << std::endl;
            closesocket(client);
            for (int i = 0; i < numClients; ++i) {
                if (clients[i] == client) {
                    // Remove disconnected client from the array
                    for (int j = i; j < numClients - 1; ++j) {
                        clients[j] = clients[j + 1];
                    }
                    numClients--;
                    break;
                }
            }
            break;
        } else {
            buffer[bytesReceived] = '\0';
            std::cout << buffer << std::endl;
            // Broadcast message to all other clients
            for (int i = 0; i < numClients; ++i) {
                if (clients[i] != client) {
                    send(clients[i], buffer, bytesReceived, 0);
                }
            }
        }
    }
}

void create_server(int port, char* ip) {
   
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    if (WSAStartup(ver, &wsData) != 0) {
        std::cerr << "Error initializing Winsock." << std::endl;
    }

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);

    if (server == INVALID_SOCKET) {
        std::cerr << "Error creating socket." << std::endl;
        WSACleanup();
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    if (bind(server, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Error binding connection." << std::endl;
        closesocket(server);
        WSACleanup();
    }

    std::cout << "Looking for clients..." << std::endl;

    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening on socket." << std::endl;
        closesocket(server);
        WSACleanup();
    }

    while (true) {
        sockaddr_in client_addr;
        int clientSize = sizeof(client_addr);
        SOCKET client = accept(server, (sockaddr*)&client_addr, &clientSize);

        if (client == INVALID_SOCKET) {
            std::cerr << "Error accepting client." << std::endl;
            continue;
        }

        if (numClients >= MAX_CLIENTS) {
            // Maximum number of clients reached, close the connection
            std::cerr << "Maximum number of clients reached. Connection closed." << std::endl;
            closesocket(client);
            continue;
        }

        // Add client to array
        clients[numClients++] = client;

        // Start receiving thread for this client
        std::thread(receiveMessages, client).detach();
    }

    closesocket(server);
    WSACleanup();
};


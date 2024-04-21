#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#include "headers.h"
#include "client.h"

#pragma comment(lib, "ws2_32.lib")

const char key[] = "123";
int portNum;
char ip[16];

int main() {
    std::cout << "Enter the IP address you want to host on:" << std::endl;
    std::cin.getline(ip, sizeof(ip));

    std::cout << "On what port:" << std::endl;
    std::cin >> portNum;

    // Start the server in a thread
    std::thread server_thread(create_server, portNum, ip);
    
    // allow some time for the server to start up
    std::this_thread::sleep_for(std::chrono::seconds(1)); // This is not always necessary but can help in avoiding immediate connection issues.

    // Start the client in thread
    std::thread client_thread(createClient, portNum, ip);

    // Join both threads
    server_thread.join();
    client_thread.join();

    return 0;
}

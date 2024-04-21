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
int choice;

int main() {
    std::cout << "Welcome to the walkie-talkie app" << std::endl << "What would you like to do?" << std::endl << "1: Create a server" << std::endl << "2: Connect to a server" << std::endl;
    std::cin >> choice;

    std::cin.ignore();  // Ignore newline character left in the input buffer

    if (choice == 1) {
        // If the user wants to host
        std::cout << "Enter the IP address you want to host on:" << std::endl;
        std::cin.getline(ip, sizeof(ip));

        std::cout << "On what port:" << std::endl;
        std::cin >> portNum;
        std::cout << "All set now, run the client." << std::endl;
        create_server(portNum, ip);
    } else if (choice == 2) {
        // If the user wants to connect
        std::cout << "Enter the IP address you want to connect to:" << std::endl;
        std::cin.getline(ip, sizeof(ip));

        std::cout << "What port are you talking on:" << std::endl;
        std::cin >> portNum;

        createClient(portNum, ip);
    } else {
        std::cout << "Invalid choice, try again." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }
    return 0;
}

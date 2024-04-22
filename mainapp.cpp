#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#include "client.h" 
#include "headers.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

string key;

int main() {
    int portNum;
    char ip[16];
    int choice;


    cout << "Welcome to the walkie-talkie app" << endl << "What would you like to do?" << endl << "1: Create a server" << endl << "2: Connect to a server" << endl;
    cin >> choice;
    cin.ignore();  


    if (choice == 1) {
        cout << "Enter the IP address you want to host on:" << endl;
        cin.getline(ip, sizeof(ip));
        cout << "On what port:" << endl;
        cin >> portNum;
        create_server(portNum,ip);
    } else if (choice == 2) {
        cout << "Enter the IP address you want to connect to:" << endl;
        cin.getline(ip, sizeof(ip));
        cout << "What port are you talking on:" << endl;
        cin >> portNum;
        cout << "What is the key to this chat?" << endl;
        cin >> key;
        createClient(portNum, ip, key);
    } else {
        cout << "Invalid choice, try again." << endl;
    }

    return 0;
}
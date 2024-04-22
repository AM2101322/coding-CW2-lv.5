#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#include "client.h"
#include "headers.h"
#include "csv.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

string key;

int main() {
    int portNum;
    char ip[16];
    int choice;
    int user_choice;
    string users_file = "user_file";
    
    cout << "Welcome to the walkie-talkie app" << endl << "Would you like to:" << endl
     << "1. Create user" << endl << "2. Login" << endl;
    cin >> user_choice;
    cin.ignore();

    if (user_choice == 1) {
        create_user(users_file);
        cout << "Thank you for creating an account." << endl;
        return 0;
    }

    if (user_choice == 2) {
        string user_username;
        string user_pass;

        bool is_verified = verify_password(users_file, user_username, user_pass);
        if (is_verified) {
            cout << "Welcome to the walkie-talkie app" << endl << "What would you like to do?" << endl;
            cout << "1: Create a server" << endl << "2: Connect to a server" << endl;
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                cout << "Enter the IP address you want to host on:" << endl;
                cin.getline(ip, sizeof(ip));
                cout << "On what port:" << endl;
                cin >> portNum;
                create_server(portNum, ip);
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
        } else {
            cout << "Password verification failed." << endl;
        }
    } else {
        cout << "Invalid choice, try again." << endl;
    }

    return 0;
}

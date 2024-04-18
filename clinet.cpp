#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    /* ---------- INITIALIZING VARIABLES ---------- */

    int client;
    int portNum = 1500; // NOTE that the port number is same for both client and server
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

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client == INVALID_SOCKET) {
        cerr << "Error creating socket." << endl;
        WSACleanup();
        return 1;
    }

    cout << "\n=> Socket client has been created..." << endl;

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);

    // Convert IP address from text to binary format
    server_addr.sin_addr.s_addr = inet_addr(ip);
    if (server_addr.sin_addr.s_addr == INADDR_NONE) {
        cerr << "Invalid address. Error: " << WSAGetLastError() << endl;
        closesocket(client);
        WSACleanup();
        return 1;
    }

    if (connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Error connecting to server." << endl;
        closesocket(client);
        WSACleanup();
        return 1;
    }

    cout << "=> Connection to the server port number: " << portNum << endl;
    cout << "=> Awaiting confirmation from the server..." << endl;

    recv(client, buffer, bufsize, 0);
    cout << "=> Connection confirmed, you are good to go..." << endl;
    cout << "\n\n=> Enter # to end the connection\n" << endl;

    do {
        cout << "Client: ";
        cin.getline(buffer, bufsize);

        send(client, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "#") == 0) {
            isExit = true;
            send(client, buffer, strlen(buffer), 0);
            break;
        }

        cout << "Server: ";
        recv(client, buffer, bufsize, 0);
        cout << buffer << endl;

        if (strcmp(buffer, "#") == 0) {
            isExit = true;
            break;
        }

    } while (!isExit);

    cout << "\n=> Connection terminated.\nGoodbye...\n";

    closesocket(client);
    WSACleanup();
    return 0;
}

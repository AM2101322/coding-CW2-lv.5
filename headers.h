#ifndef headers_H
#define headers_H

#include <iostream>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")




void receiveMessages(SOCKET client);


void create_server(int port, char* ip);



#endif // SERVER_APP_H

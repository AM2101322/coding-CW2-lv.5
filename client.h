#ifndef client_H
#define client_H

#include <WS2tcpip.h>
#include <string>




void XOR(char* buffer, int size);


void receiveMessages_C(SOCKET client);


void createClient(int portNum, const char* ip);

#endif 

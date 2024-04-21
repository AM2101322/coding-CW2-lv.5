#ifndef client_H
#define client_H

#include <WS2tcpip.h>
#include <string>



void XOR(char* buffer, int size, const std::string& key);


void receiveMessages_C(SOCKET client, const std::string& key);


void createClient(int portNum, const char* ip, const std::string& key);

#endif 

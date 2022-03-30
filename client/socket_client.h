#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string>
#include "json.hpp"
using nlohmann::json;
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

class socket_client
{
public:
    int mySocketfd;
    socket_client();
    bool ConectToServer(const char* servIP, int servPort);
    int Send(const char *buf, const int len);
    int Recv(char *buf, const int len);

    int SendJson(json& sendData);
    int RecvJson(json& recvData);
    ~socket_client();
};

#endif // SOCKET_CLIENT_H

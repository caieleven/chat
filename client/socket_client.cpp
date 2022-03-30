#include "socket_client.h"

socket_client::socket_client():mySocketfd(0)
{

}

bool socket_client::ConectToServer(const char *servIP, int servPort)
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
   mySocketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   sockaddr_in  sockAddr;
   memset(&sockAddr, 0, sizeof(sockAddr));
   sockAddr.sin_family = PF_INET;
   sockAddr.sin_addr.s_addr = inet_addr(servIP);
   sockAddr.sin_port = htons(servPort);
   if(connect(mySocketfd, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) != 0)
   {
       closesocket(mySocketfd);
       mySocketfd = 0;
       return false;
   }
   return true;
}

int socket_client::Send(const char *buf, const int len)
{
    return send(mySocketfd, buf, len + 1, 0);
}

int socket_client::Recv(char *buf, const int len)
{
    return recv(mySocketfd, buf, len, 0);
}


int socket_client::SendJson(json &sendData)
{
    std::string sendbuf = sendData.dump();
    return 1;
}

int socket_client::RecvJson(json &recvData)
{
    return 1;
}

socket_client::~socket_client()
{
    if(mySocketfd != 0)
        closesocket(mySocketfd);
    WSACleanup();
}

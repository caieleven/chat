#ifndef MSGHANDLE_H
#define MSGHANDLE_H


#include "globe.h"

#include <string.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
#include <json.hpp>
#include <sys/epoll.h>

using json = nlohmann::json;

class MsgHandle
{
public:
    int clnt_sock;
    static std::unordered_map<int, sockaddr_in> fdmap;
    //fd and username
    static std::unordered_map<std::string, int> usernamemap;
    static std::unordered_map<int, std::string> usernamemap_rev;
    MsgHandle(int clnt_sock_o);
    void registerHandle(json* recvData);
    void loginHandle(json* recvData, int epfd);
    void privateChatHandle(json* recvData);
    void groupChatHandle(json* recvData);
    void isActiveHandle(json* recvData);
    void operator()(int epfd);
};


MsgHandle::MsgHandle(int clnt_sock_o):clnt_sock(clnt_sock_o)
{

}


void MsgHandle::operator()(int epfd)
{

    char buff[50];
    std::string recvData_str;
    memset(buff, 0, sizeof(buff));
    //reev data
    while(1)
    {
        int len = recv(clnt_sock, buff, sizeof(buff) - 1, 0);
        if(len == 0)
        {
            std::cout << "disconnect\n";
            epoll_ctl(epfd, EPOLL_CTL_DEL, clnt_sock, NULL);
            close(clnt_sock);
            break;
        }
        else if(len > 0)
        {
            recvData_str.append(std::string(buff));
        }

        else
        {
            if(errno == EAGAIN)
            {
                std::cout << "read end\n";
                break;
            }
            else
            {
                perror("recv");
                exit(0);
            }
        }
        memset(buff, 0, sizeof(buff));
    }

    std::cout << recvData_str << std::endl;
    json recvData_json = json::parse(recvData_str.c_str());

    //std::cout << recvData_json["command"] << std::endl;

    if(recvData_json["command"] == "login")
        loginHandle(&recvData_json, epfd);
    else if(recvData_json["command"] == "register")
        registerHandle(&recvData_json);
    else if(recvData_json["command"] == "privatechat")
        privateChatHandle(&recvData_json);
    else if(recvData_json["command"] == "groupchat")
        groupChatHandle(&recvData_json);
    else if(recvData_json["command"] == "isactive")
        isActiveHandle(&recvData_json);


}

void MsgHandle::loginHandle(json* recvData, int epfd)
{
    std::string username = (*recvData)["username"];
    std::string password = (*recvData)["password"];
    if(!allusers.isIn(username, password))
    {
        epoll_ctl(epfd, EPOLL_CTL_DEL, clnt_sock, NULL);
        close(clnt_sock);
        return ;
    }

    currusers.insert((*recvData)["username"]);
    usernamemap.emplace((*recvData)["username"], clnt_sock);
    usernamemap_rev.emplace(clnt_sock, (*recvData)["username"]);
    //std::cout << "login reture" << std::endl;
    char buff[] = "y"; 
    send(clnt_sock, buff, sizeof(buff), 0);
}

void MsgHandle::registerHandle(json* recvData)
{
    std::cout << "enter register handle" << std::endl;
    allusers.registernew((*recvData)["username"], (*recvData)["password"]);
}

void MsgHandle::privateChatHandle(json* recvData)
{
    //std::cout << "privatechat" << std::endl;
    json sendData;
    //std::cout << "get the username" << std::endl;
    // for(auto& map:usernamemap)
    //     std::cout << map.first << " " << map.second << std::endl;
    //std::cout << sendData["username"] << std::endl;
    //std::cout << usernamemap.at((*recvData)["username"]) << std::endl;
    int rece_clnt = usernamemap.at((*recvData)["username"]);
    std::cout << rece_clnt << std::endl;
    sendData["username"] = usernamemap_rev.at(clnt_sock);
    sendData["message"] = (*recvData)["message"];
    std::cout << "begin to trans" << std::endl;
    std::string sendData_str = sendData.dump();
    std::cout << "begin to write" << std::endl;
    std::cout << write(rece_clnt, sendData_str.c_str(), sendData_str.size()) << std::endl;
    std::cout << "send success" << std::endl;
}

void MsgHandle::groupChatHandle(json* recvData)
{
    std::cout << "enter groupchar handle" << std::endl;
    json sendData;
    std::string groupname = (*recvData)["username"];
    sendData["username"] = usernamemap_rev.at(clnt_sock);
    sendData["message"] = (*recvData)["message"];
    std::string sendData_str = sendData.dump();
    for(auto tempuser:currusers.users)
    {
        if(groups.isIn(groupname,tempuser) && tempuser != usernamemap_rev.at(clnt_sock))
        {
            std::cout << "send to one user" << std::endl;
            int rece_clnt = usernamemap.at(tempuser);
            write(rece_clnt, sendData_str.c_str(), sendData_str.size());
        }
    }
}


void MsgHandle::isActiveHandle(json* recvData)
{
    std::string username = (*recvData)["username"];
    bool isAct = currusers.find(username);
    char buff[] = "y";
    if(isAct)
        send(clnt_sock, buff, sizeof(buff), 0);
    else
    {
        buff[0] = 'n';
        send(clnt_sock, buff, sizeof(buff), 0);
    }
}

#endif
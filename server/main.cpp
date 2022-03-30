
#include "thread_pool.h"
#include "globe.h"
#include "msghandle.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unordered_map>
#include <errno.h>
#include <fcntl.h>



std::unordered_map<int, sockaddr_in> MsgHandle::fdmap;
std::unordered_map<std::string, int> MsgHandle::usernamemap;
std::unordered_map<int, std::string> MsgHandle::usernamemap_rev;
Usermap allusers;
Userset currusers;
ChatGroup groups;

bool task(int clnt_sock, int epfd)
{
    MsgHandle msghandle_obj = MsgHandle(clnt_sock);
    msghandle_obj(epfd);
    return true;
}

int main(){
    //创建套接字
    /*
     * @param AF_INET 使用IPv4协议
     * @param SOCK_STREAM 面向连接的传输方式
     * @param IPPROTO_TCP 使用TCP协议

     */

    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("192.168.153.128");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口

    // 绑定各种属性
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    ThreadPool  threadpool(4);

    //进入监听状态，等待用户发起请求
    listen(serv_sock, 20);


    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    
    std::unordered_map<int, sockaddr_in> fdmap;

    int epfd = epoll_create(1);
    if(epfd == -1)
    {
        perror("epoll_create");
        exit(0);
    }

    struct epoll_event ev, evs[1024];
    int size = sizeof(evs)/sizeof(struct epoll_event);
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = serv_sock;

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &ev) < 0)
    {
        std::cout << "serv_sock add fail" << std::endl;
        return 0;
    }

    while(1)
    {
        int num = epoll_wait(epfd, evs, size, -1);
        std::cout << "====num: " << num << "====" << std::endl;
        for(int i = 0; i < num; ++i)
        {
            int curfd = evs[i].data.fd;
            
            if(curfd == serv_sock)
            {
                int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
                if(clnt_sock == -1)
                {
                    std::cout << "accept fault" << std::endl;
                    continue;
                }
                std::cout << inet_ntoa(clnt_addr.sin_addr) << ":" << ntohs(clnt_addr.sin_port) << "connected..." << std::endl;
                fdmap[clnt_sock] = clnt_addr;
                // set noblocking
                int flag = fcntl(clnt_sock, F_SETFL, O_NONBLOCK);
                ev.data.fd = clnt_sock;
                
                if(epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &ev) < 0)
                {
                    std::cout << "add error" << std::endl;
                    return 0;
                }
            }

            else
            {
                threadpool.enqueue(task, curfd, epfd);
            }

        }
    }


    // //接收客户端请求

    // std::vector<std::thread> threads_vector;
    // std::vector<handle> handle_vector;

    // while(1)
    // {

    //     int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); 
    //     std::cout << clnt_sock << std::endl;
    //     handle handleobj(clnt_sock, clnt_addr);
    //     // handle_vector.push_back(handleobj);
    //     // threads_vector.push_back(std::thread(handleobj));

    //     // threadpool.enqueue(handleobj);
    //     threadpool.enqueue(task, clnt_sock, clnt_addr);
    // }

   
    close(serv_sock);

    return 0;
}
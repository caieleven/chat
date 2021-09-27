//#include "widget.h"
#include "start.h"
#include "socket_client.h"
#include "contacts.h"
#include <QApplication>
#include "thread_pool.h"

ThreadPool threadpool(6);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Widget w;
    //w.show();
//    Contacts mainwindow;
//    mainwindow.show();
//    Widget w;
//    w.show();
//    socket_client clnt_sock = socket_client();
//    clnt_sock.ConectToServer("192.168.153.128", 1234);
//    //Start start(0, &clnt_sock);
//    Start start(0, &clnt_sock);
//    start.show();
    Contacts mainwindow;
    return a.exec();
}

#include "widget.h"
#include "start.h"
#include "socket_client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Widget w;
    //w.show();
    socket_client clnt_sock = socket_client();
    clnt_sock.ConectToServer("192.168.153.128", 1234);

    Start start(0, &clnt_sock);
    start.show();
//    Widget w;
//    w.show();
    return a.exec();
}

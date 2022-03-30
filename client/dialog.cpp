#include "dialog.h"
#include "ui_dialog.h"


dialog::dialog(QWidget *parent, socket_client* clnt_sock_o, std::string username_o, std::string me_o) :
    QWidget(parent),username_(username_o),clnt_sock(clnt_sock_o),me_(me_o),
    ui(new Ui::dialog)
{
    ui->setupUi(this);
    if(username_ != "R206")
    {
        bool active = isActive();
        //输出信息
        printActMsg(active);
    }
    //判断该用户是否上线

    //recvAndPrintMsg();
    //threadpool.enqueue(std::bind(&dialog::recvAndPrintMsg, this));
//    std::thread th(std::bind(&dialog::recvAndPrintMsg, this));
//    th.detach();
//    while(1)
//    {
//        on_send_clicked();

//    }
}

dialog::~dialog()
{
    delete ui;

}

void dialog::closeEvent(QCloseEvent *)
{
    emit this->closedialog();
}


bool dialog::isActive()
{
    json sendmsg;
    sendmsg["command"] = "isactive";
    sendmsg["username"] = username_;
    std::string sendmsg_str = sendmsg.dump();
    clnt_sock->Send(sendmsg_str.c_str(), sendmsg_str.size());
    char recvbuf[3];
    memset(recvbuf, 0, sizeof(recvbuf));
    clnt_sock->Recv(recvbuf, sizeof(recvbuf));
    if(recvbuf[0] == 'y')
        return true;
    return false;
}


void dialog::printActMsg(bool isAct)
{
    QString state;
    if(isAct)
        state = "在线";
    else
        state = "离线";
    QString username = QString::fromStdString(username_);
    ui->msgBrowser->setTextColor(Qt::red);
    ui->msgBrowser->append(" " + username + " " + state);
}



void dialog::printmsg(json& msg)
{
    //获取当前时间
    QString curtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    std::string tempusername = msg["username"];
    std::string tempmessage = msg["message"];
    QString username = QString(tempusername.c_str());
    QString message = QString(tempmessage.c_str());
    ui->msgBrowser->setTextColor(Qt::blue);
    ui->msgBrowser->append("[" + username + "]" + curtime);
    ui->msgBrowser->append(message);
}


bool dialog::recvAndPrintMsg()
{
    char recvbuff[120];
    while(1)
    {
        //ui->msgBrowser->append("执行一次");
        if(clnt_sock->Recv(recvbuff, 120)!=0)
        {
            json msg = json::parse(recvbuff);
            QString curtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            std::string tempusername = msg["username"];
            std::string tempmessage = msg["message"];
            QString username = QString(tempusername.c_str());
            QString message = QString(tempmessage.c_str());
            ui->msgBrowser->setTextColor(Qt::blue);
            ui->msgBrowser->append("[" + username + "]" + curtime);
            ui->msgBrowser->append(message);
        }
        else
            break;
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    //没有实际意义
    return true;

}




void dialog::on_send_clicked()
{
    if(ui->textMsg->toPlainText() == "")
        return;
    json sendData;
    if(username_ == "R206")
    {
        sendData["command"] = "groupchat";
        sendData["username"] = username_;
    }
    else
    {
        sendData["command"] = "privatechat";
        sendData["username"] = username_;
    }

    sendData["message"] = ui->textMsg->toPlainText().toStdString();
    ui->textMsg->clear();
    std::string sendData_str = sendData.dump();
    clnt_sock->Send(sendData_str.c_str(), sendData_str.size());

    sendData["username"] = me_;
    printmsg(sendData);
}


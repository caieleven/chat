#include "start.h"
#include "ui_start.h"

Start::Start(QWidget *parent, socket_client* clnt_sock) :
    QWidget(parent),
    ui(new Ui::Start),
    clnt_sock_(clnt_sock)
{
    ui->setupUi(this);
}

Start::~Start()
{
    delete ui;
}


void Start::SendUserInfo(std::string& cmd)
{
    json msg;
    //msg["command"] = "register";
    QString username = ui->username->text();
    QString password = ui->password->text();
    msg["command"] = cmd;
    msg["username"] = username.toStdString();
    msg["password"] = password.toStdString();
    std::string sendbuf = msg.dump();
    clnt_sock_->Send(sendbuf.c_str(), sendbuf.size());
}
void Start::on_register_2_clicked()
{
    std::string cmd("register");
    SendUserInfo(cmd);
}


void Start::on_login_clicked()
{
    std::string cmd("login");
    SendUserInfo(cmd);
    char buf[5];
    if(clnt_sock_->Recv(buf, sizeof(buf)) > 0)
    {
        std::string username = ui->username->text().toStdString();
        emit sendUsername(username);
        this->close();
    }

}


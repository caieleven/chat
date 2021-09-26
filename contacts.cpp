#include "contacts.h"
#include "ui_contacts.h"
#include "QToolButton"

Contacts::Contacts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Contacts)
{
    ui->setupUi(this);
    clnt_sock = socket_client();
    clnt_sock.ConectToServer("192.168.153.128", 1234);
    //Start start(0, &clnt_sock);
    startwindow = new Start(0, &clnt_sock);
    startwindow->show();
    connect(startwindow, SIGNAL(sendUsername(std::string)), this, SLOT(getUsername(std::string)));

    QList<QString> nameList;
    nameList << "R206" << "zhu" << "bu" << "yan";

    QVector<QToolButton*> toolBtns;
    {
        QToolButton *btn = new QToolButton;
        btn->setText(nameList[0]);
        ui->verticalLayout_groups->addWidget(btn);
        toolBtns.push_back(btn);
    }

    for(int i = 1; i < 4; ++i)
    {
        QToolButton *btn = new QToolButton;
        btn->setText(nameList[i]);
        ui->verticalLayout_friends->addWidget(btn);
        toolBtns.push_back(btn);
    }




}

Contacts::~Contacts()
{
    delete ui;
}

void Contacts::getUsername(std::string user_name)
{
    this->username = user_name;
    this->show();
}

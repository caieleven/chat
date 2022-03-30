#include "contacts.h"
#include "ui_contacts.h"

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

    //应从服务端获取namelist
    QList<QString> nameList;
    nameList << "R206" << "zhu" << "bu" << "yan";

//    QVector<QToolButton*> toolBtns;
    {
        QToolButton *btn = new QToolButton;
        btn->setText(nameList[0]);
        ui->verticalLayout_groups->addWidget(btn);
        toolBtns.push_back(btn);
        isShow.push_back(false);
    }

    for(int i = 1; i < 4; ++i)
    {
        QToolButton *btn = new QToolButton;
        btn->setText(nameList[i]);
        ui->verticalLayout_friends->addWidget(btn);
        toolBtns.push_back(btn);
        isShow.push_back(false);
    }

    //添加信号槽


    for(int i = 0; i < 4; ++i)
    {
        connect(toolBtns[i], &QToolButton::clicked, [=]()
        {
            if(isShow[i])
            {
                QString str = QString("%1窗口已经打开").arg(toolBtns[i]->text());
                QMessageBox::warning(this, "提示", str);
                return;
            }

            isShow[i] = true;
            dialog* mydialog = new dialog(0, &clnt_sock, toolBtns[i]->text().toStdString(), username);
            mydialog->setWindowTitle(toolBtns[i]->text());
            mydialog->show();

            connect(mydialog, &dialog::closedialog, [=]()
            {
                isShow[i] = false;
            });
        });
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

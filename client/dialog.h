#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include "socket_client.h"
#include <QCloseEvent>
#include <QDateTime>
#include "json.hpp"
#include "thread_pool.h"

namespace Ui {
class dialog;
}

class dialog : public QWidget
{
    Q_OBJECT

public:
    explicit dialog(QWidget *parent = nullptr, socket_client* clnt_sock = nullptr, std::string username_o = "", std::string me_o = "");
    ~dialog();
    socket_client* clnt_sock;
    bool isActive();
    void printmsg(json& msg);
    bool recvAndPrintMsg();

private:
    Ui::dialog *ui;
    std::string username_;
    std::string me_;
    void printActMsg(bool);


signals:
    void closedialog();
public:
    void closeEvent(QCloseEvent *);


private slots:
    void on_send_clicked();
};

#endif // DIALOG_H

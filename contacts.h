#ifndef CONTACTS_H
#define CONTACTS_H

#include <QWidget>
#include "socket_client.h"
#include "start.h"
#include "qtoolbutton.h"
#include "dialog.h"
#include "QMessageBox"

namespace Ui {
class Contacts;
}

class Contacts : public QWidget
{
    Q_OBJECT

public:
    explicit Contacts(QWidget *parent = nullptr);
    ~Contacts();
    Start* startwindow;
    QVector<QToolButton*> toolBtns;
    QVector<bool> isShow;

private slots:
    void getUsername(std::string);

private:
    Ui::Contacts *ui;
    socket_client clnt_sock;
    std::string username;

};

#endif // CONTACTS_H

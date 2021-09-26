#ifndef START_H
#define START_H
#include "socket_client.h"
#include "json.hpp"
#include <QWidget>
#include <string>
namespace Ui {
class Start;
}

using nlohmann::json;

class Start : public QWidget
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = nullptr, socket_client* clnt_sock = nullptr);
    ~Start();
    void SendUserInfo(std::string& cmd);

private slots:
    void on_register_2_clicked();

    void on_login_clicked();
signals:
    void sendUsername(std::string);

private:
    Ui::Start *ui;
    socket_client *clnt_sock_;
};

#endif // START_H

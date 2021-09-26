#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>

namespace Ui {
class dialog;
}

class dialog : public QWidget
{
    Q_OBJECT

public:
    explicit dialog(QWidget *parent = nullptr);
    ~dialog();

private:
    Ui::dialog *ui;
};

#endif // DIALOG_H

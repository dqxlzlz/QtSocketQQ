#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "clientsocket.h"
#include <QWidget>

namespace Ui {
class chatWindow;
}

class chatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit chatWindow(QWidget *parent = nullptr);
    void init();
    ~chatWindow();

private:
    Ui::chatWindow *ui;
    clientSocket* tcp;
    QString sender;
    QString acept;
};

#endif // CHATWINDOW_H

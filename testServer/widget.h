#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <tcpserver.h>
namespace Ui {
class widget;
}

class widget : public QWidget
{
    Q_OBJECT

public:
    explicit widget(QWidget *parent = nullptr);
    ~widget();
private slots:
   // void SltMsg();
    void SltdispalyUserStatus(QString text);
private:
    Ui::widget *ui;
    tcpServer *tcpMsg;


};

#endif // WIDGET_H

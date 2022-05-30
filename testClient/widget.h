#ifndef WIDGET_H
#define WIDGET_H

#include "homepage.h"
#include <QWidget>
#include "clientsocket.h"
#include <QLabel>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QJsonObject>
#include <QMessageBox>

#include "windowchat.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

//    事件过滤器,使label能够获得点击效果
    bool eventFilter(QObject *object,QEvent *eve);

    //鼠标事件，按下、释放、移动事件
    void mousePressEvent(QMouseEvent *eve);
    void mouseReleaseEvent(QMouseEvent* eve);
    void mouseMoveEvent(QMouseEvent* eve);
    bool mousePress;    //鼠标是否点击
    QPoint movePoint;  //鼠标移动的距离
    bool isLetterNum(QString str);//判断是否全由数字与字母组成
private slots:    
    //鼠标事件
    //注册、返回、登陆按钮
    void on_pbtRegister_clicked();
    void on_pbtReturn_clicked();
    void on_pbtLogin_clicked();
private slots:
    //根据type值来判断是否注册或者是登陆成功
    void SltTcpStatus(int type);
private:
    Ui::Widget *ui;
    bool isConnected;
    clientSocket *tcp;
    //图片背景
    QImage *img;
    QImage *img1;
    QImage *img2;
    HomePage *HMPage;
    QMessageBox msgBox;
    windowChat *test;
};

#endif // WIDGET_H

#ifndef WINDOWCHAT_H
#define WINDOWCHAT_H

#include <QWidget>
#include "clientsocket.h"
#include <QMouseEvent>


namespace Ui {
class windowChat;
}

class windowChat : public QWidget
{
    Q_OBJECT

public:
    explicit windowChat(QWidget *parent = nullptr);
    void init(QString m_sender,QString m_acept,QString NickName,QString friendNickName);  //初始化配置信息
    ~windowChat();
    int getStatus();
    QString getACepterAccount();
    void AppendMessage(const QString sender,const QString content);
    bool eventFilter(QObject *object,QEvent *eve);

    //鼠标事件，按下、释放、移动事件
    void mousePressEvent(QMouseEvent *eve);
    void mouseReleaseEvent(QMouseEvent* eve);
    void mouseMoveEvent(QMouseEvent* eve);
    bool mousePress;    //鼠标是否点击
    QPoint movePoint;  //鼠标移动的距离
private slots:
    void on_pbtSend_clicked();
    void on_pbtCloseLow_clicked();

signals:
    void signalCloseChatWindow();//关闭窗口信号
private:
    clientSocket* tcp;
    QString sender,acept;
    QString myNickName,friNickName;
    Ui::windowChat *ui;
    int status;
    QImage *img;
};

#endif // WINDOWCHAT_H

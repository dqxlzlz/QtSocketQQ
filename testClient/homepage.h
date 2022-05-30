#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include "windowchat.h"
#include "addfriend.h"
#include "clientsocket.h"
#include "set.h"

struct friendStatus
{
public:
    QString friAccount;
    QString friNickName;
    QString status;
};
namespace Ui {
class HomePage;
}

class friendInfo
{
    QLabel friNickName;
    QLabel friAccount;
    QLabel firStatus;
    QLabel friSignate;
};

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();
    //初始化socket配置
    void SetSocket(const QString &account,const int &CId );
    bool eventFilter(QObject *object,QEvent *eve);
    //void getUserInfo();
    void getFriInfo();
    void tcpConnectToServer();
    void closeAll();     //关闭全部窗口
    //鼠标事件，按下、释放、移动事件
    void mousePressEvent(QMouseEvent *eve);
    void mouseReleaseEvent(QMouseEvent* eve);
    void mouseMoveEvent(QMouseEvent* eve);
    bool mousePress;    //鼠标是否点击
    QPoint movePoint;  //鼠标移动的距离
public slots:
    void SLtSetFriInfo(QString,QString,QString,QString);
    void SLtSet(int type,QString signteToSend);//获取用户在修改签名窗口的内容
    void SltChatMessage(const QString  senderAccount,const QString senderNickName,const QString content);
private slots:
    void SltNickName(QString strName);
    void SltSignate(QString strName);
    void getUserInfo();
    void SltCloseCharWindow();
private:
    Ui::HomePage *ui;
    clientSocket *tcp;    //通信套字
    QImage *img;
    QString myAccount;    //用户账号
    QString myNickName;   //用户昵称
    addFriend *pageAddFri;  //添加好友页面
    class friendInfo* fri;  //获取好友信息
    QList<QLabel*> friendList;  //好友列表
    QList<QLabel*> AvatorList;  //头像列表
    QList<windowChat*> chatWinList;  //聊天窗口
    QList<friendStatus*>friStatusList; //好友状态
    QStringList friAcc;         //好友账号
    QMessageBox msgBox;
    QString friendStatus;       //好友状态
    QTimer *m_pTimer;
    QImage *imgOffLine = new QImage();
    QImage *imgOnLine = new QImage();
    class set* SetOfWindow;
};

#endif // HOMEPAGE_H

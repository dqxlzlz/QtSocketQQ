#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtNetwork>

class clientSocket:public QObject
{
    Q_OBJECT
public:
    explicit clientSocket(QObject *parent = nullptr);
    ~clientSocket();
    //与服务器相连接
    void ConnectToHost(const QString &host,const int &port);
    void ConnectToHost(const QHostAddress &host,const int &port);
    void ConnectToHost();
    //获取、修改用户的socketid
    int GetUserId() const;
    void SetUserId(const int &id);

   //检查是否已连接与关闭连接
    void checkConnected();
    void closeConnected();

signals:
    void signalMessage(const quint8 &type,const QJsonValue &dataVal);
    void signalStatus(int state);                  //
    void signalNickName(QString name);             //将获取的昵称以信号的形式发送，在homepage中的槽函数处理
    void signalSignate(QString signate);          //将获取的签名以信号的形式发送，在homepage中的槽函数处理
    void signalAddFriend(int type);               //发出是否成功添加好友的信号，在homepage中的槽函数处理
    void signalFriendInfo(QString FriendStatus,QString FriendAccount,QString FriendNickName,QString FriendSignate);
    void signalChatMessage(const QString senderAccount,const QString senderNickName,const QString content);
public slots:
    //连接、断连
   // void SltSendOnline();
//    void SltSendOffline();
    void SltSendMessage(int type,const QJsonValue &dataVal);   //发送消息给服务器
private:
   //解析登陆返回信息
    void parseLogin(const QJsonValue &dataVal);//解析登陆返回信息
    void parseRegister(const QJsonValue &dataVal);//解析注册返回信息
    void parseGetUserInfo(const QJsonValue &dataVal);//解析获取用户信息的返回信息
    void parseAddFriend(const QJsonValue &dataVal);//解析是否成功添加好友的返回信息
    void parseFriendInfo(const QJsonValue &dataVal);//解析好友信息的返回信息
    void parseChatMessage(const QJsonValue &dataVal);//解析聊天的返回信息
private slots:
    //与服务器连接、断开、接受消息
   // void SltDisconnected();
   // void SltConneted();
    void SltReadyRead();   //读取到服务器发送的消息

//成员变量
private:
    QTcpSocket *tcpsocket;
    int ClientId;        //用于记录当前的客户端对应的账号
};

#endif // CLIENTSOCKET_H

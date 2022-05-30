#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "database.h"
#include "databasetest.h"


class clientsocket : public QObject
{
    Q_OBJECT
public:
    explicit clientsocket(QObject *parent = nullptr,QTcpSocket *tcpsocket = nullptr);
    ~clientsocket();

    //获取与设置当前的serverId
    int GetUserId() const;
    void SetUserId(int SId);
    void close();

signals:
    //自定义槽函数，在客户端成功验证账号密码时发出
    void signalConnected(const int& id,QString account);
    void signalDisConnected(QString account);
    void signalChatMessage(int type,const int& id,const QJsonValue &dataVal);

public slots:
    void SltSendMessage(int type,const QJsonValue &json);// 发送消息至客户端

private slots:
    void SLtDisconnected(QString account);
    void SltConnected();
    //当有消息传入时，调用槽函数
    void SltRead();

private:
    QTcpSocket *tcp;
    int ServerId;

    //解析客户端发送来的各种请求
    void ParseLogin(const QJsonValue &dataVal);
    void ParseRegister(const QJsonValue &dataVal);
    void ParseGetUserInfo(const int& source,const QJsonValue &dataVal);
    void ParseAddFriend(const QJsonValue &dataVal);
    void ParseGetFriendList(const QJsonValue &dataVal);
    void ParseLogout(const int& source,const QJsonValue &dataVal);
    void ParseMessage(const int& source,const QJsonValue &dataVal);
    void ParseConnectServer(const int& source,const QJsonValue &dataVal);
    void ParseSetSignte(const QJsonValue &dataVal);

    class databaseTest *data;

};

#endif // CLIENTSOCKET_H

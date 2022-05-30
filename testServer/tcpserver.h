#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QVector>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "clientsocket.h"
#include "database.h"

class tcpServer : public QObject
{
    Q_OBJECT
public:
    explicit tcpServer(QObject *parent = nullptr);
    ~tcpServer();
    //默认的端口号为8888
    bool Openlisten(int port = 8888);
    void closeListen();
signals:
    void SignalStatusClient(QString text);
protected:
    QTcpServer *tcp;
public slots:
    void SltNewConnection() ;
    void SltConnected(const int& id,QString account) ;
    void SltMsgToClient(int type,const int &id,const QJsonValue &json);
    void SLtDisconnected(QString account);
//    virtual void sltDisConnected() = 0;
private:
    QVector<clientsocket *> clients;//用向量类管理登陆成功的客户端

};
#endif // TCPSERVER_H

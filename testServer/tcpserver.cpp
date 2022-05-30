#include "tcpserver.h"


#include <QHostAddress>
#include <QMessageBox>

tcpServer::tcpServer(QObject *parent):QObject (parent)
{
//    有新连接
    tcp = new QTcpServer(this);
    connect(tcp,SIGNAL(newConnection()),this,SLOT(SltNewConnection()));

}
tcpServer::~tcpServer()
{
//    关闭监听
    if(tcp->isListening())
        tcp->close();
    qDebug()<<"close all";
    foreach(clientsocket *client,clients)
    {
        clients.removeOne(client);
        client->close();
    }
}
bool tcpServer::Openlisten(int port)
{
    if(tcp->isListening())tcp->close();
    bool bOK = tcp->listen(QHostAddress::Any,port);
    return bOK;
}
void tcpServer::closeListen()
{
    tcp->close();
}
//TcpMsgServer::TcpMsgServer(QObject *parent):tcpServer (parent)
//{

//}
/*
TcpMsgServer::~TcpMsgServer()
{
    qDebug()<<"close all";
    foreach(clientsocket *client,clients)
    {
        clients.removeOne(client);
        client->close();
    }
}*/
void tcpServer::SltNewConnection()
{
//    将连接的客户端取出，放进定义好的clientsocket链表中
    clientsocket *client = new clientsocket(this,tcp->nextPendingConnection());
    qDebug()<<"连接成功,连接用户的id为:"<<client->GetUserId();
    connect(client,SIGNAL(signalChatMessage(int,const int&,const QJsonValue&)),this,SLOT(SltMsgToClient(int,const int&,const QJsonValue&)));
    connect(client,SIGNAL(signalConnected(const int&,QString)),this,SLOT(SltConnected(const int&,QString)));
    connect(client,SIGNAL(signalDisConnected(QString)),this,SLOT(SLtDisconnected(QString)));
}

void tcpServer:: SltMsgToClient(int type,const int &id,const QJsonValue &json)
{
    qDebug()<<"进入了msgtoClients";
    for(int i=0;i<clients.size();i++)
    {
        qDebug()<<"进入第"<<i<<"次匹配";
        if(id == clients.at(i)->GetUserId())
        {
            clients.at(i)->SltSendMessage(type,json);
        }
    }
}
void tcpServer :: SltConnected(const int &id,QString account)
{
    qDebug()<<account<<"登陆了";
    clientsocket *client = (clientsocket *)this->sender();//发送这个信号的客户端
    if(client == nullptr)
        return ;
    qDebug()<<"client的id为"<<client->GetUserId();
    for(int i=0;i<clients.size();i++)
    {
        if(clients[i]->GetUserId() == id)
            return ;
    }
    emit SignalStatusClient(QString("用户账号：[%1]上线").arg(account));
    clients.push_back(client);
    qDebug()<<"clients[0]的nId为"<<clients[0]->GetUserId();
    qDebug()<<"QVector<clients>的大小为"<<clients.size();
}
void tcpServer :: SLtDisconnected(QString account)
{
    qDebug()<<account<<"登出了";
    clientsocket *client = (clientsocket *)this->sender();//发送这个信号的客户端
    if(client == nullptr)
        return ;
    qDebug()<<"登出用户的的nId为"<<client->GetUserId();
    for(int i=0;i<clients.size();i++)
    {
        qDebug()<<"正在匹配第"<<i<<"个用户";
        if(client->GetUserId() == clients.at(i)->GetUserId())
        {
            qDebug()<<"匹配成功登陆用户tcp信息成功";
            clients.remove(i);
            Q_EMIT SignalStatusClient(QString("用户账号 [%10] 下线").arg(account));
        }
    }
}


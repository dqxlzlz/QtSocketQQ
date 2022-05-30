#include "clientsocket.h"
#include <database.h>

clientSocket::clientSocket(QObject *parent):
    QObject(parent)
{
    //数据成员的初始化
    ClientId = -1;
    tcpsocket = new QTcpSocket(this);
    //将默认的事件连接到对应的槽函数
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(SltReadyRead()));
    connect(tcpsocket,SIGNAL(connected()),this,SLOT(SltConnected()));
    connect(tcpsocket,SIGNAL(disconnect()),this,SLOT(SltDisconnected()));
}

//断开连接
clientSocket::~clientSocket()
{
//    SltSendOffline();
}
void clientSocket::SetUserId(const int &id)
{
    ClientId = id;
}
int clientSocket::GetUserId() const
{
    return ClientId;
}

void clientSocket::checkConnected()
{
    if(tcpsocket->state() != QTcpSocket::ConnectedState)
    {   //测试版，在运行时，便固定是“192.168.31.251”，8888
        tcpsocket->connectToHost("192.168.31.251",8888);
    }
}
void clientSocket::closeConnected()
{
    if(tcpsocket->isOpen()) tcpsocket->abort();
}
void clientSocket::ConnectToHost(const QString &host, const int &port)
{   //如果已打开，则关闭
    if(tcpsocket->isOpen())tcpsocket->abort();
    //否者则连接
    tcpsocket->connectToHost(host,port);

}
//overload()
void clientSocket::ConnectToHost(const QHostAddress &host, const int &port)
{//如果已打开，则关闭
    if(tcpsocket->isOpen())tcpsocket->abort();
    //否者则连接
    tcpsocket->connectToHost(host,port);
}

void clientSocket:: ConnectToHost()
{
    QString localHostName = QHostInfo :: localHostName();  //获取本机ip地址
    qDebug()<<"localHostName"<<localHostName;
    QHostInfo info = QHostInfo::fromName(localHostName);
    qDebug()<<"本机所有的ip地址为："<<info.addresses();
    QString ipv4Address;
    foreach(QHostAddress address,info.addresses())    //获取本地的ipv地址
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            ipv4Address = address.toString();
            qDebug()<<"ipv4地址为："<<ipv4Address;
            break;
        }
    }
    tcpsocket->connectToHost(ipv4Address,8888);
}
//判断传过去的类型type、传送到值val
void clientSocket:: SltSendMessage(int type,const QJsonValue &dataVal)
{
//    如发送消息至服务器
    qDebug()<<"send login message"<<type;
    QJsonObject json;
    json.insert("type",type);
    qDebug()<<"type:"<<type<<" from"<<ClientId;
    json.insert("from",ClientId);
    json.insert("data",dataVal);
//    通过json文档的形式传送值服务器，
    QJsonDocument docu;
//    将要发送的内容添加到json文档中
    docu.setObject(json);
    tcpsocket->write(docu.toJson(QJsonDocument::Compact));
}
//从服务器端读取消息
void clientSocket::SltReadyRead()
{
    qDebug()<<"point2";
    QByteArray  Read =tcpsocket->readAll();
//    判断解析时是否发生错误
    QJsonParseError jsonError;
//      检验是否有错
    QJsonDocument document = QJsonDocument::fromJson(Read,&jsonError);
//    文档不为空，而且没有错误
    if(!document.isNull() &&  (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject jsonObj = document.object();
            //获取其中的数据成员
            QJsonValue dataVal = jsonObj.value("data");
            //获取其中的消息类型
            int typ = jsonObj.value("type").toInt();
            qDebug()<<"客户端读到的消息类型为："<<typ;
            /*
              1为返回的注册消息
              2为返回的登陆消息
              3为返回的获取用户信息消息
              4为添加好友的消息
              5为获取好友列表
              7为获取到聊天消息
            */

            switch(typ)
            {
            //1表示消息为注册消息
                case 1://1为返回的注册消息
                {
//            解析返回的注册是否成功的消息
                    parseRegister(dataVal);
                    break;
                }
                case 2://2为返回的登陆消息
                {
                    qDebug()<<"point1";
                    parseLogin(dataVal);
                    break;
                }
                case 3://3为获取用户消息
                {
                    qDebug()<<"进入了case3";
                    parseGetUserInfo(dataVal);
                    break;
                }
                case 4://4为添加好友的消息
                {
                   parseAddFriend(dataVal);
                   break;
                }
                case 5: // 5为获取好友列表
                {
                    parseFriendInfo(dataVal);
                    break;
                }
                case 7:  //7为获取到聊天消息
                {
                    parseChatMessage(dataVal);
                    break;
                }
            }
        }
    }
}
//解析返回的注册验证消息
void clientSocket::parseRegister(const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        ClientId = dataObj.value("id").toInt();
//      如果得到的CId值为-1，表明注册失败，否则注册成功
        if(ClientId != -1)
        {
            //10表示注册成功，11表示注册失败
            qDebug()<<"注册成功";
            Q_EMIT signalStatus(10);//发送注册成功信号
        }
        else
        {
            qDebug()<<"注册失败";
            Q_EMIT signalStatus(11);//发送注册失败信号
        }
    }
}
//解析返回的登陆消息
void clientSocket::parseLogin(const QJsonValue &dataVal)
{
//    消息有效
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        int judge = dataObj.value("id").toInt();//获取是否成功登陆
        qDebug()<<"获取到的用户id为";
        if(judge == -1)
        {
            Q_EMIT signalStatus(13); //13表示账号或密码错误，发送信号
        }
        else if(judge == -2)
        {
            Q_EMIT signalStatus(14);//14表示重复登陆，发送信号
        }
        else {
            ClientId = judge;
            qDebug()<<"信息匹配";
            Q_EMIT signalStatus(12); //12表示登陆成功，发送信号
        }
    }
}
void clientSocket:: parseGetUserInfo(const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        QString strName = dataObj.value("NickName").toString();//获取用户昵称
        QString strSignate = dataObj.value("signate").toString();//获取用户签名
        //发送信号，在homepage的槽函数中处理
        emit signalNickName(strName);
        emit signalSignate(strSignate);
    }
}
void clientSocket :: parseAddFriend(const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        int result = dataObj.value("result").toInt();//获取是否成功添加好友
        //发送信号，在addfriend中的槽函数处理
        emit signalAddFriend(result);
    }
}

void clientSocket::parseFriendInfo(const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        //获取好友是否在线、账号、昵称、签名
        QString strFriendStatus = dataObj.value("status").toString();
        QString strFriendAccount = dataObj.value("account").toString();
        QString strFriendNickName = dataObj.value("NickName").toString();
        QString strFriendSignate = dataObj.value("signate").toString();

        qDebug()<<"好友的状态"<<strFriendStatus;
        qDebug()<<"好友的账号"<<strFriendAccount;
        //以信号的形式将数据发送，在homepage中的槽函数处理
        emit signalFriendInfo(strFriendStatus,strFriendAccount,strFriendNickName,strFriendSignate);
    }
}

void clientSocket::parseChatMessage(const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        qDebug()<<"成功收到了聊天消息";
        QJsonObject dataObj = dataVal.toObject();
        QString strSenderAccount = dataObj.value("senderAccount").toString();
        QString strSenderNickName = dataObj.value("senderNickName").toString();
        QString strContent = dataObj.value("content").toString();
        Q_EMIT signalChatMessage(strSenderAccount,strSenderNickName,strContent);
        qDebug()<<"消息的发送者为"<<strSenderAccount;
        qDebug()<<"消息的内容为为"<<strContent;
    }
}

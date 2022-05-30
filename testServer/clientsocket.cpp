#include "clientsocket.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

clientsocket::clientsocket(QObject *parent,QTcpSocket *tcpsocket):QObject (parent)
{
    //初始化数据
    ServerId = -1;

    if(tcp == nullptr) tcp = new QTcpSocket(this);
    tcp = tcpsocket;
    data = new databaseTest();
    //打开数据库
    data->openDB();
    data->createTable();
    //关联信号与事件
    connect(tcp,SIGNAL(readyRead()),this,SLOT(SltRead()));
    connect(tcp,SIGNAL(connected()),this,SLOT(SltConnected()));
    connect(tcp,SIGNAL(disconnected()),this,SLOT(SltDisconnected()));

}
clientsocket::~clientsocket()
{

}

void clientsocket::SltConnected()
{

}

void clientsocket::SLtDisconnected(QString( account))
{

}

int clientsocket::GetUserId() const
{
    return ServerId;
}

void clientsocket::SetUserId(int SId)
{
    ServerId = SId;
}

void clientsocket::close()
{
    tcp->abort();
}

void clientsocket::SltRead()
{

//读取消息，同时判断json是否有错
    QByteArray reply = tcp->readAll();
    QJsonParseError jsonError;
    QJsonDocument docu = QJsonDocument::fromJson(reply,&jsonError);

    //    如果消息无错误
    if(!docu.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(docu.isObject())
        {
           /*1表示注册
             2表示登陆
             3表示获取用户的信息，用户名、签名
             4表示添加好友请求
             5为获取好友列表与好友的状态
             6表示登出
             7为收发消息
             8为用户登陆成功消息
            */
            QJsonObject jsonObj = docu.object();
            //根据typ类型来判断客户端发送过来的数据
            int typ = jsonObj.value("type").toInt();
            qDebug()<<"解析出来的type"<<typ;
            int dataFrom = jsonObj.value("from").toInt();
            QJsonValue dataVal = jsonObj.value("data");
            switch (typ) {
                case 1://1表示注册
                {
                    ParseRegister(dataVal);
                    break;
                }
                case 2://2表示登陆
                {
                    ParseLogin(dataVal);
                    break;
                }
                case 3://3表示获取用户的信息，用户名、签名、好友列表
                {
                    ParseGetUserInfo(dataFrom,dataVal);
                    break;
                }
                case 4://4表示添加好友请求
                {
                    qDebug()<<"选择了parse函数";
                    ParseAddFriend(dataVal);
                    break;
                }
                case 5://5为获取好友列表
                {
                    qDebug()<<"into getfriendlist！！！！！！！！！";
                    ParseGetFriendList(dataVal);
                    break;
                }
                case 6://6表示登出
                {
                    ParseLogout(dataFrom,dataVal);
                    break;
                }
                case 7://7为收发消息
                {
                    ParseMessage(dataFrom,dataVal);
                    break;
                }
                case 8://8为用户登陆成功消息
                {
                    qDebug()<<"进入了登陆成功界面";
                    //ParseConnectServer(dataFrom,dataVal);  改的时候发现用不上
                }
                case 9:
                {
                    qDebug()<<"进入修改页面";
                   ParseSetSignte(dataVal);
                }
            }
        }
    }
    else {
       qDebug()<<"我也不知道这是一个什么错误,反正就是进不去";
    }
}
//发送消息
void clientsocket::SltSendMessage(int type,const QJsonValue &jsonval)
{
    if(!tcp->isOpen())return;
//   将所有要传输的数据打包进入一个json对象中，发送至客户端
    QJsonObject jsonObj;
    jsonObj.insert("type",type);
    jsonObj.insert("from",ServerId);
    jsonObj.insert("data",jsonval);
    QJsonDocument document;
    document.setObject(jsonObj);
    tcp->write(document.toJson(QJsonDocument::Compact));
}

//解析注册消息
void clientsocket::ParseRegister(const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        //从json对象中提取出账号密码
        QString strName  = dataObj.value("account").toString();
        QString strPwd = dataObj.value("password").toString();
        QString strNickName = dataObj.value("NickName").toString();
        qDebug()<<"注册账号："<<dataObj.value("account").toString();
        qDebug()<<"注册密码："<<dataObj.value("password").toString();
        qDebug()<<"注册的昵称"<<dataObj.value("NickName").toString();
        //获取是否成功注册，赋给judge，然后返回至客户端
        int judge = data->insertAccount(strName,strPwd,strNickName);
        qDebug()<<"judge:"<<judge;
        QJsonObject json;
        json.insert("id",judge);
//       将是否成功注册账号的消息返回至客户端，其中账号、密码用json来储存
        SltSendMessage(1,json);
    }
}

void clientsocket::ParseLogin(const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        QString strAccount = dataObj.value("account").toString();
        QString strPwd = dataObj.value("password").toString();
        qDebug()<<"要登陆的账号为"<<dataObj.value("account").toString();
        qDebug()<<"要登陆的密码为"<<dataObj.value("password").toString();
        //如果账号密码不正确，则ServerId仍为-1，若正确但已经登陆，则为-2，若正确且为登陆，则为一个大于0的数
        //将这个数返回至客户端
        ServerId = data->checkLogin(strAccount,strPwd);
        qDebug()<<"查询出来的用户ID为"<<ServerId;
        QJsonObject json;
        json.insert("id",ServerId);
        SltSendMessage(2,json);

        //登陆成功则发送连接信号
//        if(ServerId != -1 && ServerId != -2)
//            Q_EMIT signalConnected(strAccount);
    }
}
void clientsocket::ParseGetUserInfo(const int& source,const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        QString strAccount = dataObj.value("account").toString();
        qDebug()<<"get info about"<<strAccount;
        //将获取到的结果返回至一个JsonObject，将其发送至客户端
        QJsonObject result = data->GetInfo(strAccount);
        qDebug()<<"result"<<result;
        SltSendMessage(3,result);
        ServerId = source;
        Q_EMIT signalConnected(source,strAccount);
    }

}
void clientsocket::ParseAddFriend(const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        QString strAccSender = dataObj.value("accSender").toString();
        QString strAccAcept = dataObj.value("accAcept").toString();
        qDebug()<<"发送请求的用户"<<strAccSender;
        qDebug()<<"接受请求的用户"<<strAccAcept;
        //将是否添加成功以int类型的值返回
        //0表示账号信息有误，1表示已经是好友了，2表示添加成功,3表示目标是自己
        int result = data->addFriend(strAccSender,strAccAcept);
        QJsonObject jsonObj;
        jsonObj.insert("result",result);
        SltSendMessage(4,jsonObj);
    }
}

void clientsocket :: ParseGetFriendList(const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        QString strAccount = dataObj.value("account").toString();
        //将一个好友的昵称、签名、状态返回至一个JsonObject
        //所有的好友的JsonObject组成一个QList
        QList<QJsonObject> friList = data->GetFriendInfo(strAccount);
        QString strFriStatus;
        QString strFriendAccount;
        QString strFriendNickName;
        QString strFriendSignate;
        //将好友的信息组成一个字符串，组件用‘|’隔开，方便在客户端解析
        for(int i=0;i<friList.size();i++)
        {
            strFriStatus.append(friList[i].value("status").toString());
            strFriendAccount.append(friList[i].value("account").toString());
            strFriendAccount.append("|");
            strFriendNickName.append(friList[i].value("NickName").toString());
            strFriendNickName.append("|");
            strFriendSignate.append(friList[i].value("signate").toString());
            strFriendSignate.append("|");
        }
        //将这些信息打包为一个jsonObject发送至客户端
        QJsonObject jsonObj;
        jsonObj.insert("status",strFriStatus);
        jsonObj.insert("account",strFriendAccount);
        jsonObj.insert("NickName",strFriendNickName);
        jsonObj.insert("signate",strFriendSignate);
        SltSendMessage(5,jsonObj);
    }
}

void clientsocket::ParseLogout(const int &source, const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        //更改用户在数据库中的status
        QJsonObject dataObj = dataVal.toObject();
        QString strAccount = dataObj.value("account").toString();
        int strFrom = source;
        qDebug()<<"得到的from为"<<strFrom;
        ServerId = strFrom;
        qDebug()<<"接受登出消息后，tcp的serverId为"<<ServerId;
        data->logout(strAccount);
        //在tcpserveer中显示出用户下线的信息
        Q_EMIT signalDisConnected(strAccount);
    }
}

void clientsocket::ParseMessage(const int &source, const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        QString strAccept = dataObj.value("acepter").toString();
      //  QString contentToSender = dataObj.value("content").toString();
      //  QString sender = dataObj.value("sender").toString();
        int aceptId = data->GetUserId(strAccept);
        Q_EMIT signalChatMessage(7,aceptId,dataVal);
    }
}
void clientsocket::ParseConnectServer(const int &source, const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        ServerId = source;
        QJsonObject dataObj = dataVal.toObject();
        QString strAccount = dataObj.value("account").toString();
       // Q_EMIT Q_EMIT signalConnected(strAccount);
    }

}

void clientsocket::ParseSetSignte(const QJsonValue &dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject dataObj = dataVal.toObject();
        QString strAccount = dataObj.value("account").toString();
        int type = dataObj.value("type").toInt();
        QString strSignate = dataObj.value("signate").toString();
        data->setMessage(strAccount,type,strSignate);
    }
}

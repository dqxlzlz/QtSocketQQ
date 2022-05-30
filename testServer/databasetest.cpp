#include "databasetest.h"

databaseTest::databaseTest(QObject *parent):
    QObject (parent)
{
    //构造函数中创建数据库
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("DBServer9.db");
}
bool databaseTest::openDB()
{
    //打开数据库
    if(!DB.open())
    {
        qDebug()<<"failed init database";
        return false;
    }
    else {
        qDebug()<<"suceess init database";
        return true;
    }
}
void databaseTest::createTable()
{
    //在数据库中创建表，id、账号、密码、昵称、签名、好友、是否在线
    QSqlQuery query;
    QString createSql = QString("create table UserInformation(id int primary key,account QString,password QString,NickName QString,signate QString,friends QString,status QString)");
    query.prepare(createSql);
    if(query.exec())
        qDebug()<<"success to create a table";
}
int databaseTest::searchAcc(QString acc)
{
    int SId = 0;
    QSqlQuery sqlQuery;
    //查询是否已经存在该账号
    sqlQuery.exec("select id from UserInformation WHERE account = '"+acc+"'");
    if(sqlQuery.next())
    {
        //找到了几返回-1，表明数据库中存在该账号成员
        qDebug()<<"exit";
        return -1;
    }
    else {
        qDebug()<<"account no exits";
    }
     sqlQuery.exec("select id from UserInformation order by id DESC");//寻找最高id
     if(sqlQuery.next())
     {
         SId = sqlQuery.value("id").toInt();
         qDebug()<<"目前最高id为"<<SId;
     }
     else {
        qDebug()<<"max id = 0";
     }
     return SId;
}
int databaseTest::insertAccount( QString acco, QString psd,QString NickName)
{
    int CId = searchAcc(acco);
     if(CId == -1)//searchAcc的结果为-1，表明账号已经存在，不能注册，返回-1
    {
        qDebug()<<"账号已存在";
        return -1;
    }
    else {
         //否则表明账号不存在，可以注册账号
        QSqlQuery sqlQuery;
        sqlQuery.prepare("insert into UserInformation (id,account,password,NickName,status) values(?,?,?,?,?)");
        sqlQuery.bindValue(0,CId+1);//在目前最高id的下一为注册账号
        sqlQuery.bindValue(1,acco);
        sqlQuery.bindValue(2,psd);
        sqlQuery.bindValue(3,NickName);
        sqlQuery.bindValue(4,"0");  //初始时，用户未登陆
        if(sqlQuery.exec())
            qDebug()<<"success";
        else {
            qDebug()<<"fail"<<sqlQuery.lastError();
        }
        return CId+1;
    }
}
databaseTest::~databaseTest()
{
    DB.close();
}

int databaseTest :: checkLogin(const QString &account,const QString &psd)
{
    //检查账号密码是否匹配
    QString strQuery = "select id,status from UserInformation WHERE account='";
    strQuery.append(account);
    strQuery.append("' AND password='");
    strQuery.append(psd);
    strQuery.append("';");

    QSqlQuery query(strQuery);
    int SId;
    if(query.next())
    {
        //判断是否已经登陆
        if(QString :: compare(query.value("status").toString(),"0") ==0)
        {
            qDebug()<<"要登陆的账号的id为"<<query.value("id").toInt();
            SId = query.value("id").toInt();
            //找到了，就修改status，使之为1，表示已经登陆
            const QString instatus = "1";
            strQuery = "update UserInformation set status = '";
            strQuery.append(instatus);
            strQuery.append("' WHERE account = '");
            strQuery.append(account);
            strQuery.append("'");
            if(query.exec(strQuery))
                qDebug()<<"成功执行了替换状态的操作";
            else {
                qDebug()<<"fail to execute"<<query.lastError();
            }
        }
        else {
            SId = -2; //-2代表重复登陆
        }
    }
    else
    {
        qDebug()<<"不匹配";
        SId = -1;
    }
    return SId;
}
QJsonObject databaseTest :: GetInfo(QString account)
{
    //根据账号消息查询用户信息
    QString strQuery = QString("select NickName,signate,friends from UserInformation WHERE account ='");
    strQuery.append(account);
    strQuery.append("'");
    //已经经过了登陆验证了，所以一定能查询出用户信息
    QSqlQuery sqlQuery;
    sqlQuery.exec(strQuery);
    if(sqlQuery.next())
        qDebug()<<"找到这个b了";
    else {
        qDebug()<<"没找到"<<sqlQuery.lastError();
    }
    //将信息打包为一个JsonObject发送至客户端
    QString strName = sqlQuery.value("NickName").toString();
    QString strSignate = sqlQuery.value("signate").toString();
    QJsonObject jsonObj;
    jsonObj.insert("NickName",strName);
    jsonObj.insert("signate",strSignate);
    return jsonObj;
}

QStringList databaseTest::getFriendList(QString account)
{
    //根据账号信息来获取好友列表
    QString strQuery = QString("select friends from UserInformation WHERE account ='");
    strQuery.append(account);
    strQuery.append("'");
    QSqlQuery sqlQuery;
    if(sqlQuery.exec(strQuery))
    {
        qDebug()<<"success to get friendList";
    }
    else
    {
        qDebug()<<"fail to get friendList"<<sqlQuery.lastError();
    }
    sqlQuery.next();
    QString infoFriend = sqlQuery.value("friends").toString();
    qDebug()<<"该账号的好友有："<<infoFriend;
    //‘|’为分隔符
    QStringList FriList = infoFriend.split('|');
    qDebug()<<FriList.size();
   // for(int i=0;i<FriList.size())
    return FriList;
}

bool databaseTest::isFriend(QString accSender, QString accAcept)
{
    QStringList friList = getFriendList(accSender);
    for(int i=0;i<friList.size()-1;i++) //最后会比目标list多一个“”的空字符串
    {
        qDebug()<<"执行了该循环";
        if(QString::compare(accAcept,friList.at(i)) == 0)  //在发送者的好友列表里面找到了他，表明他们已经是好友了
            return true;
    }
    return false;
}

int databaseTest :: GetUserId(QString account)
{
    //获取用户的id信息
    QString strQuery = QString("select id from UserInformation WHERE account ='");
    strQuery.append(account);
    strQuery.append("'");
    QSqlQuery sqlQuery;
    sqlQuery.exec(strQuery);
    sqlQuery.next();
    qDebug()<<"得到的用户id为："<<sqlQuery.value("id").toInt();
    return sqlQuery.value("id").toInt();
}
int databaseTest :: addFriend(QString accSender, QString accAcept)
{
    //0表示账号信息有误，1表示已经是好友了，2表示添加成功,3表示目标是自己
    if(QString::compare(accAcept,accSender) == 0)
    {
        qDebug()<<"不能添加自己为好友";
        return 3;
    }
    if(searchAcc(accAcept)==-1)  //找到了目标账号
    {
        if(isFriend(accSender,accAcept))
        {
            qDebug()<<"你们已经是好友了，不用再添加";
            return 1;
        }
        // 获取当前发送者的好友列表
        QString strQuery = QString("select friends from UserInformation WHERE account ='");
        strQuery.append(accSender);
        strQuery.append("'");
        QSqlQuery sqlQuery;
        sqlQuery.exec(strQuery);
        sqlQuery.next();  //发送消息的人一定是通过了登陆验证，表明账号一定的存在的
        QString strFriends = sqlQuery.value("friends").toString();
        //在当前的好友列表的末尾上加上目标用户的账号以及‘|’
        strFriends.append(accAcept);
        strFriends.append("|");
        qDebug()<<strFriends;
        //更新数据库表中的friends
        strQuery = QString("update UserInformation set friends = '");
        strQuery.append(strFriends);
        strQuery.append("'");
        strQuery.append(" WHERE account = '");
        strQuery.append(accSender);
        strQuery.append("'");
        qDebug()<<strQuery;
//        执行
        if(sqlQuery.exec(strQuery))
        {
            qDebug()<<"success add friends";

        }
        else
        {
            qDebug()<<"fial to add friends";
        }
//        同时更新接收者的好友列表
        //获取接收者好友列表
        strQuery = QString("select friends from UserInformation WHERE account ='");
        strQuery.append(accAcept);
        strQuery.append("'");
        sqlQuery.exec(strQuery);
        sqlQuery.next();
        strFriends = sqlQuery.value("friends").toString();//好友列表
//        在好友列表的末尾加上发送者的账号信息
        strFriends.append(accSender);
        strFriends.append("|");
        //更新好友列表
        strQuery = QString("update UserInformation set friends = '");
        strQuery.append(strFriends);
        strQuery.append("' WHERE account = '");
        strQuery.append(accAcept);
        strQuery.append("'");
        qDebug()<<sqlQuery.exec(strQuery);
        if(sqlQuery.exec(strQuery))
        {
            qDebug()<<"success add firend (acept)";
            return 2;
        }
    }
    else
    {
        qDebug()<<"账号不存在";
        return 0;
    }
}

QList<QJsonObject> databaseTest :: GetFriendInfo(QString account)
{
    QStringList fri = getFriendList(account);
    QList<QJsonObject> friList;
    //逐个获取好友的 昵称、账号、签名、状态
    for(int i=0;i<fri.size()-1;i++)
    {
        QJsonObject jsonObj;
        jsonObj.insert("account",fri.at(i));
        QString strQuery = QString("select id,NickName,signate,status from UserInformation WHERE account ='");
        strQuery.append(fri.at(i));
        strQuery.append("'");
        QSqlQuery sqlQuery;
        sqlQuery.exec(strQuery);
        sqlQuery.next();
        QString strFriStatus = sqlQuery.value("status").toString();
        QString strNickName = sqlQuery.value("NickName").toString();
        QString strSignate = sqlQuery.value("signate").toString();
        jsonObj.insert("status",strFriStatus);
        jsonObj.insert("NickName",strNickName);
        jsonObj.insert("signate",strSignate);
        friList.push_back(jsonObj);
    }
    return friList;
}
void databaseTest :: logout(const QString &account)
{
    //登出时，将自己的状态设置为未登录
    const QString outStatus = "0";
    QString strQuery = QString("update UserInformation set status = '");
    strQuery.append(outStatus);
    strQuery.append("' WHERE account = '");
    strQuery.append(account);
    strQuery.append("'");
    QSqlQuery sqlQuery;
    sqlQuery.exec(strQuery);
}

void databaseTest:: setMessage(const QString &account,const int type,const QString &content)
{
    if(type == 1)
    {
        QString strQuery = QString("update UserInformation set password = '");
        strQuery.append(content);
        strQuery.append("' WHERE account ='");
        strQuery.append(account);
        strQuery.append("'");
        QSqlQuery sqlQuery;
        if(sqlQuery.exec(strQuery))
            qDebug()<<"成功修改了"<<account<<"的密码"<<content;
        else {
            qDebug()<<"修改失败"<<sqlQuery.lastError();
        }
    }
    else if(type == 2)
    {
        QString strQuery = QString("update UserInformation set NickName = '");
        strQuery.append(content);
        strQuery.append("' WHERE account ='");
        strQuery.append(account);
        strQuery.append("'");
        QSqlQuery sqlQuery;
        if(sqlQuery.exec(strQuery))
            qDebug()<<"成功修改了"<<account<<"的昵称"<<content;
        else {
            qDebug()<<"修改失败"<<sqlQuery.lastError();
        }
    }
    else //修改用户的签名
    {
        QString strQuery = QString("update UserInformation set signate = '");
        strQuery.append(content);
        strQuery.append("' WHERE account ='");
        strQuery.append(account);
        strQuery.append("'");
        QSqlQuery sqlQuery;
        if(sqlQuery.exec(strQuery))
            qDebug()<<"成功修改了"<<account<<"的签名"<<content;
        else {
            qDebug()<<"修改失败"<<sqlQuery.lastError();
        }
    }
}

#ifndef DATABASETEST_H
#define DATABASETEST_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class databaseTest : public QObject
{
    Q_OBJECT
public:
    explicit databaseTest(QObject *parent = nullptr);
    ~databaseTest();
    bool openDB();    //打开数据库
    void createTable();//创建表
    int insertAccount(QString acco,QString psd,QString NickName);//注册用户时使用的函数
//    int registerUser(const QString &name,const QString &psd);  //暂时不要的函数
    int searchAcc(QString acc);   //搜索目标用户是否存在的函数
    int checkLogin(const QString &account,const QString &psd);//登陆函数
    int addFriend(QString accSender,QString accAcept);//添加好友函数
    int GetUserId(QString account);   //获取用户的账号对应的id的函数
    bool isFriend(QString accSender,QString accAcept);//判断两人是否为好友关系
    void logout(const QString &account);  //登出函数
    void setMessage(const QString &account,const int type,const QString &signate);//修改用户的个人信息
    QJsonObject GetInfo(QString account);  //获取用户的昵称、签名的函数
    QStringList getFriendList(QString account);  //获取用户的好友列表的函数
    QList<QJsonObject> GetFriendInfo(QString account); //获取用户的全部的好友是否在线、昵称、签名的函数

private:
    QSqlDatabase DB;
};

#endif // DATABASETEST_H

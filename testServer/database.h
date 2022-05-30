#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QMessageBox>

class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = nullptr);
    ~database();

  //  bool OpenDB(const QString &dataName);
  //  void CloseDB();

  /*  static database *Instance()
    {
        static QMutex mutex;
        if(NULL == self)
            QMutexLocker locker(&mutex);
        if(!self)
            self = new database();
        return self;
    }
    */
    void cretaeConnecttion();
    int RegisterUser(const QString &name,const QString &password);
    QJsonObject checkUserLogin(const QString &name,const QString &password);
private:
    static database *self;
    QSqlDatabase userDB;
};

#endif // DATABASE_H

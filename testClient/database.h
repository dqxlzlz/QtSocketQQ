#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>

class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QObject * parent = nullptr);
    ~database();
    bool OpenUserDB(const QString &dataName);
    static database *Instance()
    {
        static QMutex mutex;
        if(self == nullptr)
        {
            QMutexLocker locker(&mutex);
            if(!self)
            {
                self = new database();
            }
        }
    }

private:
    static database *self;
    QSqlDatabase userDB;
};

#endif // DATABASE_H

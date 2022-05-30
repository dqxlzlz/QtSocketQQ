#include "database.h"
#include <QDebug>

database *database :: self = nullptr;
database::database(QObject *parent):
    QObject (parent)
{

}
database::~database()
{
    if(userDB.isOpen())
        userDB.close();
}
bool database::OpenUserDB(const QString &dataName)
{
    userDB = QSqlDatabase::addDatabase("QSQLITE","connectionUser");
    userDB.setDatabaseName(dataName);
    if(!userDB.open())
    {
        qDebug()<<"failed to open database";
        return false;
    }
    QSqlQuery query(userDB);
    query.exec("CREATE TABLE USERINFO (id INT, name varchar(50), passwd varchar(50)");

}

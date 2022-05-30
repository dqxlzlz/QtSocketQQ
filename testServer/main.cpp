#include "widget.h"
#include "database.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    加载数据库
   // database::Instance()->OpenDB("info1.db");

    widget w;
    w.show();

    return a.exec();
}

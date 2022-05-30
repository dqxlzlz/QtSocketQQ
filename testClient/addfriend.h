//添加好友页面

#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>
#include "clientsocket.h"
#include <QMessageBox>

namespace Ui {
class addFriend;
}

class addFriend : public QWidget
{
    Q_OBJECT

public:
    explicit addFriend(QWidget *parent = nullptr);
    void init(const QString &account,const int &CId);//初始化配置信息
    int getStatus();
    ~addFriend();

private slots:

    void on_pbtSearch_clicked();
    //槽函数，处理是否成功添加好友
    void SltAddFriend(int type);
    void on_pbtClose_clicked();

private:
    Ui::addFriend *ui;
    clientSocket *tcp;
    QString account; //记录当前用户的账号
    QMessageBox msgBox;
    int status;
};

#endif // ADDFRIEND_H

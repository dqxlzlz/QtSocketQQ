#include "addfriend.h"
#include "ui_addfriend.h"

addFriend::addFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addFriend)
{
    ui->setupUi(this);
    //初始化数据
    status = 0;
}

addFriend::~addFriend()
{
    tcp->closeConnected();
    delete ui;
}

void addFriend::on_pbtSearch_clicked()
{
    //将自己的账号与接受者的账号一起发送至服务器，判断两人是否已经是好友
    QString accAcept = ui->lineEdit->text();
    QJsonObject jsonObj;
    jsonObj.insert("accSender",account);
    jsonObj.insert("accAcept",accAcept);
    qDebug()<<account;
    qDebug()<<accAcept;
    tcp->SltSendMessage(4,jsonObj);
}
void addFriend:: init(const QString &m_account, const int &CId)
{
    //初始化配置信息，连接至服务器，
    tcp = new clientSocket();
    tcp->ConnectToHost();
    connect(tcp,SIGNAL(signalAddFriend(int)),this,SLOT(SltAddFriend(int)));

    status = 1;
    account = m_account;
    tcp->SetUserId(CId);
}

void addFriend :: SltAddFriend(int type)
{
    //根据type的返回值确定是否成功添加
    if(type == 0)
        msgBox.setText("输入的账号有误，请重新输入");
    else if(type == 1)
        msgBox.setText("你们已经是好友了，无需重新添加");
    else if(type==2)  msgBox.setText("添加成功");
    else msgBox.setText("不能添加自己");
    msgBox.exec();
}

void addFriend::on_pbtClose_clicked()
{
    //关闭该页面
    close();
}

int addFriend::getStatus()
{
    return status;
}

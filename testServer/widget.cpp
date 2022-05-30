#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
widget::widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{
    ui->setupUi(this);
    this->setWindowTitle("这个是服务器");
    tcpMsg = new tcpServer(this);
    bool temp = tcpMsg->Openlisten(8888);
    if(temp)
    {
        qDebug()<<"listen ok!";
        ui->textEdit->append("开始监听端口号：8888！\n");
    }
    connect(tcpMsg,SIGNAL(SignalStatusClient(QString)),this,SLOT(SltdispalyUserStatus(QString)));
}
void widget:: SltdispalyUserStatus(QString text)
{
    qDebug()<<"sltDisplay";
    ui->textEdit->append(text);
}
widget::~widget()
{
    delete ui;
}

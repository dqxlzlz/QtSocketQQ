#include "setting.h"
#include "ui_setsignal.h"

setSignal::setSignal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setSignal)
{
    ui->setupUi(this);
}

setSignal::~setSignal()
{
    delete ui;
}

void setSignal::on_pbtClear_clicked()
{
    ui->textEdit->clear();
}

void setSignal::on_pbtReturn_clicked()
{

    close();
}

/*
    1代表修改密码
    2代表修改昵称
    3代表修改签名
*/
void setSignal::on_pbtOk_clicked()
{
    QString signiteToSend = ui->textEdit->toPlainText();
    Q_EMIT signalToSend(3,signiteToSend);
    close();
}

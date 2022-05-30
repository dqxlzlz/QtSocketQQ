#include "set.h"
#include "ui_set.h"
#include <QDebug>

set::set(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::set)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
}

set::~set()
{
    delete ui;
}

int set::GetStatus()
{
    return status;
}

void set::SetStatus(int temp)
{
    status = temp;
}
void set::on_pbtPasswod_clicked()
{
    QString NewPassword = ui->lineEditPassword->text();
    Q_EMIT siganlMessageToSend(1,NewPassword);
    ui->lineEditPassword->clear();
}

void set::on_pbtNickName_clicked()
{
    qDebug()<<"获取了nickname信息";
    QString NewNickName = ui->lineNickName->text();
    Q_EMIT siganlMessageToSend(2,NewNickName);
    ui->lineNickName->clear();
}

void set::on_pbtSignate_clicked()
{
    QString NewSignate = ui->textEditSignte->toPlainText();
    Q_EMIT siganlMessageToSend(3,NewSignate);
    ui->textEditSignte->clear();
}

void set::on_pbtClear_clicked()
{
    ui->lineEditPassword->clear();
    ui->lineNickName->clear();
    ui->textEditSignte->clear();
}

void set::on_pbtReturn_clicked()
{
    status = 0;
    close();
}

void set::mousePressEvent(QMouseEvent *eve)
{
    if(eve->button() == Qt::LeftButton)//鼠标左击事件
    {
        //追踪鼠标的位置
        this->setMouseTracking(true);
        mousePress = true;
        //获取当前是鼠标位置
        movePoint = eve->pos();
    }
}
void set::mouseReleaseEvent(QMouseEvent *eve)
{
    mousePress = false;
}
void set::mouseMoveEvent(QMouseEvent *eve)
{
    if(mousePress)
    {
        //获取鼠标相对与全屏的位置
        QPoint move_pos = eve->globalPos();
        this->move(move_pos - movePoint);
    }
}

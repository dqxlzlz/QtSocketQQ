#include "windowchat.h"
#include "ui_windowchat.h"

windowChat::windowChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::windowChat)
{
    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);
    tcp = new clientSocket();
    img = new QImage();
    //加载背景图片
    img->load(":/picture/qqshow.png");
    ui->label_3->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/emojy.png");
    ui->labelEmojy->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/file.png");
    ui->labelfile->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/more.png");
    ui->labelMore->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/cut.png");
    ui->labelCut->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/close.png");
    ui->labelCLose->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/min.png");
    ui->labelMin->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/max.png");
    ui->labelMax->setPixmap(QPixmap::fromImage(*img));

    ui->labelCLose->setStyleSheet(":hover{background-color:red;}");
    ui->labelMin->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelMax->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelEmojy->setStyleSheet("QLabel:hover{background-color:rgb(241,241,241,50%);}");
    ui->labelfile->setStyleSheet("QLabel:hover{background-color:rgb(241,241,241,50%);}");
    ui->labelCut->setStyleSheet("QLabel:hover{background-color:rgb(241,241,241,50%);}");
    ui->labelMore->setStyleSheet("QLabel:hover{background-color:rgb(241,241,241,50%);}");

    ui->labelCLose->installEventFilter(this);
    ui->labelMax->installEventFilter(this);
    ui->labelMin->installEventFilter(this);

    ui->labelNickName->setAlignment(Qt::AlignCenter);
    ui->widgetTop->setStyleSheet("QWidget{color:white;background:rgb(18,183,245)}");
    status = 0;

    ui->textEditRead->setReadOnly(true);
}

void windowChat::init(QString m_sender, QString m_acept,QString NickName,QString friendNickName)
{
    tcp->ConnectToHost();
    sender = m_sender;
    acept = m_acept;
    myNickName = NickName;
    friNickName = friendNickName;
    status = 1;
    ui->labelNickName->setText(friendNickName);
}
int windowChat:: getStatus()
{
    return status;
}
windowChat::~windowChat()
{
    delete ui;
}

void windowChat::on_pbtSend_clicked()
{
    QString contentToSend = ui->textEditReadSend->toPlainText();
    qDebug()<<contentToSend;
    QJsonObject jsonObj;
    jsonObj.insert("content",contentToSend);
    jsonObj.insert("senderAccount",sender);
    jsonObj.insert("senderNickName",myNickName);
    jsonObj.insert("acepter",acept);
    tcp->SltSendMessage(7,jsonObj); //7为收发消息
    QString str;
    str.append("[");
    str.append(myNickName);
    str.append("]");
    str.append(":");
    str.append(contentToSend);
    ui->textEditRead->append(str);
    ui->textEditReadSend->clear();
}

void windowChat::on_pbtCloseLow_clicked()
{
    Q_EMIT signalCloseChatWindow();
    close();
}

QString windowChat :: getACepterAccount()
{
    return acept;
}
void windowChat :: AppendMessage(const QString senderNickName,const QString content)
{
    QString strContent;
    strContent.append("[");
    strContent.append(senderNickName);
    strContent.append("]");
    strContent.append(":");
    strContent.append(content);
    ui->textEditRead->append(strContent);
}

bool windowChat::eventFilter(QObject *object, QEvent *eve)
{
    if(eve->type() == QEvent::MouseButtonPress && object == ui->labelCLose)
    {
        Q_EMIT signalCloseChatWindow();
        close();
    }
    else if(eve->type() == QEvent::MouseButtonPress && object == ui->labelMax)
    {
        showMaximized();
    }
    else if(eve->type() == QEvent::MouseButtonPress && object == ui->labelMin)
    {
        showMinimized();
    }
    return false;
}
void windowChat :: mousePressEvent(QMouseEvent *eve)
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
void windowChat::mouseReleaseEvent(QMouseEvent *eve)
{
    mousePress = false;
}
void windowChat :: mouseMoveEvent(QMouseEvent *eve)
{
    if(mousePress)
    {
        //获取鼠标相对与全屏的位置
        QPoint move_pos = eve->globalPos();
        this->move(move_pos - movePoint);
    }
}

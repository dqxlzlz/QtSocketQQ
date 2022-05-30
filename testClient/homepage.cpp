#include "homepage.h"
#include "ui_homepage.h"
#define TIME_TIMEOUT (2*1000)

HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{

//    加载背景图标
    ui->setupUi(this);
    SetOfWindow = new set();
    setWindowFlag(Qt::FramelessWindowHint);
    pageAddFri= new addFriend();
    m_pTimer = new QTimer();
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(getUserInfo()));
    connect(SetOfWindow,SIGNAL(siganlMessageToSend(int,QString)),this,SLOT(SLtSet(int,QString)));
    m_pTimer->start(TIME_TIMEOUT);

    img = new QImage();
    imgOnLine = new QImage();
    imgOffLine = new QImage();
    img->load(":/picture/top.jpg");
    ui->labelTopPic->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/room.png");
    ui->labelIconKj->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/set.png");
    ui->labelIconSetting->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/search.png");
    ui->labelSearch->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/muea.png");
    ui->labelIconMeau->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/addFriend.png");
    ui->labelIconAddFri->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/wheter.png");
    ui->labelIconWhether->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/candidate.png");
    ui->labelIconcandator->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/AvatroPic.jpg");
    ui->labelAvator->setPixmap(QPixmap::fromImage(*img));

    //图标悬停时添加背景
    ui->labelMin->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelIcon->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelIconAddFri->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelIconMeau->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelIconWhether->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelIconcandator->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelIconKj->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelIconSetting->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelClose->setStyleSheet("QLabel:hover{background-color:red;}");
    //获取用户的昵称与签名

    //产生点击效果
    ui->labelClose->installEventFilter(this);
    ui->labelMin->installEventFilter(this);
    ui->labelIconAddFri->installEventFilter(this);
    ui->labelIcon->installEventFilter(this);
    ui->labelIconSetting->installEventFilter(this);
//    ui->labelRefresh->installEventFilter(this);
    //    ui->label->installEventFilter(this);
}

HomePage::~HomePage()
{
    delete ui;
}
void HomePage :: SetSocket(const QString &account, const int &CId)
{
    tcp = new clientSocket();
    qDebug()<<"homePage connect";
    tcp->ConnectToHost();
    tcp->SetUserId(CId);

    connect(tcp,SIGNAL(signalNickName(QString)),this,SLOT(SltNickName(QString)));
    connect(tcp,SIGNAL(signalSignate(QString)),this,SLOT(SltSignate(QString)));
    connect(tcp,SIGNAL(signalFriendInfo(QString,QString,QString,QString)),this,SLOT(SLtSetFriInfo(QString,QString,QString,QString)));
    connect(tcp,SIGNAL(signalChatMessage(const QString,const QString,const QString)),this,SLOT(SltChatMessage(const QString,const QString,const QString)));
    myAccount = account;
    getUserInfo();
}

void HomePage :: SltNickName(QString strName)
{
    qDebug()<<"打开了槽函数name";
    myNickName = strName;
    ui->labelNickName->setText(strName);
}

void HomePage :: SltSignate(QString strSignate)
{
    if(strSignate != "")
    {
        ui->labelSginal->setText(strSignate);
    }
    else {
       ui->labelSginal->setText("单击上方的设置按钮以修改签名");
    }
    getFriInfo();
}

bool HomePage::eventFilter(QObject *object, QEvent *eve)
{
    if(eve->type() == QEvent::MouseButtonPress && object == ui->labelMin)
        showMinimized();
    if(eve->type() == QEvent::MouseButtonPress && object == ui->labelClose)
    {
        closeAll();
    }
    else if(eve->type() == QEvent::MouseButtonPress && object == ui->labelIcon)
    {
        qDebug()<<"进入了获取用户信息的事件中";
        getUserInfo();
    }

    else if(eve->type() == QEvent::MouseButtonPress && object == ui->labelIconAddFri)
    {
        pageAddFri->init(myAccount,tcp->GetUserId());
        qDebug()<<"进入了evenfilter";
        pageAddFri->show();
    }
    else if(eve->type() == QEvent::MouseButtonPress && object == ui->labelIconSetting)
    {
        qDebug()<<"进入了set evenfilter";
        SetOfWindow->SetStatus(1);
        SetOfWindow -> show();
    }
    else
    {
        for(int i=0;i<friendList.size();i++)
        {
            if(eve->type() == QEvent::MouseButtonPress && object == friendList[i])
            {
                qDebug()<<"进入了list:"<<i;
                if(QString::compare(friStatusList[i]->status,"0") == 0)
                {
                    qDebug()<<"没在线";
                    msgBox.setText("他还没上线，等他上线再聊吧");
                    msgBox.exec();
                }
                else {
                    qDebug()<<"在线";
                   windowChat* winChat = new windowChat();
                   winChat->init(myAccount,friStatusList[i]->friAccount,myNickName,friStatusList[i]->friNickName);
                   winChat->show();
                   connect(winChat,SIGNAL(signalCloseChatWindow()),this,SLOT(SltCloseCharWindow()));
                   chatWinList.push_back(winChat);
                }
            }
        }
    }
    return false;
}

void HomePage:: tcpConnectToServer()
{
    QJsonObject jsonObj;
    jsonObj.insert("account",myAccount);
    tcp->SltSendMessage(8,jsonObj);
}

void HomePage :: getUserInfo()
{
    QJsonObject json;
    json.insert("account",myAccount);
    tcp->SltSendMessage(3,json);
}

void HomePage::SltCloseCharWindow()
{
    windowChat *winChat = (windowChat*)this->sender();
    for(int i=0;i<chatWinList.size();i++)
    {
        if(QString::compare(chatWinList[i]->getACepterAccount(),winChat->getACepterAccount()) == 0)
        {
            qDebug()<<"找到了这一项"<<i;
            chatWinList.removeAt(i);
            qDebug()<<"现在有这么多个聊天窗口:"<<chatWinList.size();
        }
    }
}

void HomePage :: getFriInfo()
{
    QJsonObject json;
    json.insert("account",myAccount);
    tcp->SltSendMessage(5,json);
}

void HomePage :: SLtSetFriInfo(QString strFriStatus,QString strFriAccount,QString strFriNickName,QString strFriSignate)
{
    imgOnLine->load(":/picture/online.jpg");
    imgOffLine->load(":/picture/ofline.jpg");
    QStringList friAccount = strFriAccount.split('|');
    //friAcc = friAccount;
    QStringList friNickName = strFriNickName.split('|');
    QStringList friSignate = strFriSignate.split('|');
    QString info ;
    qDebug()<<"size of str"<<friAccount.size();
    for(int i=0;i<friAccount.size()-1;i++)
    {
        friendList.push_back(new QLabel(this));
        AvatorList.push_back(new QLabel(this));
        info.append(friNickName.at(i));
        info.append("    (");
        info.append(friAccount.at(i));
        info.append(")\n");
        info.append(friSignate.at(i));
        if(QString::compare(strFriStatus.at(i),"0") == 0)
        {
            info.append("    [离线]");
            AvatorList[i]->setPixmap(QPixmap::fromImage(*imgOffLine));
        }
        else
        {
            info.append("    [在线]");
            AvatorList[i]->setPixmap(QPixmap::fromImage(*imgOnLine));
        }
        qDebug()<<info;
        friendList[i]->setText(info);
        friendList[i]->setWordWrap(true);
        info.clear();
        AvatorList[i]->setGeometry(0,250+i*75,75,75);
        AvatorList[i]->show();
        friendList[i]->setGeometry(75,250+i*75,275,75);
        friendList[i]->setStyleSheet("QLabel{background:rgb(255,255,255)}");
        friendList[i]->show();
        friendList[i]->installEventFilter(this);
        friStatusList.push_back(new struct friendStatus);
        friStatusList[i]->friAccount = friAccount.at(i);
        friStatusList[i]->status = strFriStatus.at(i);
        friStatusList[i]->friNickName = friNickName.at(i);
    }
}

void HomePage::closeAll()
{
    QJsonObject jsonObj;
    jsonObj.insert("account",myAccount);
    tcp->SltSendMessage(6,jsonObj);
    for(int i = 0;i<chatWinList.size();i++)
    {
        if(chatWinList[i]->getStatus() == 1)
        {
            chatWinList[i]->close();
        }
    }
    if(pageAddFri->getStatus() == 1)
    {
        pageAddFri->close();
    }
    if(SetOfWindow->GetStatus() == 1)
    {
        SetOfWindow -> close();
    }
    close();
}

void HomePage::SltChatMessage(const QString senderAccount,const QString senderNickName,const QString content)
{
    qDebug()<<"进入了接收消息的槽函数";
    //判断是否存在该聊天窗口，如果存在，就在该窗口添加聊天消息
    for(int i=0;i<chatWinList.size();i++)
    {
        qDebug()<<"发送消息的人的account为："<<chatWinList[i]->getACepterAccount();
        qDebug()<<"聊天窗口的页面的account为"<<senderAccount;
        if(QString :: compare(chatWinList[i]->getACepterAccount(),senderAccount) == 0)
        {
            chatWinList[i]->AppendMessage(senderNickName,content);
            return;
        }
    }
    //如果没找到，就打开新的聊天窗口，在聊天窗口处显示消息
    windowChat* winChat = new windowChat();
    winChat->init(myAccount,senderAccount,myNickName,senderNickName);
    qDebug()<<"没找到对应的聊天窗口";
    winChat->show();
    connect(winChat,SIGNAL(signalCloseChatWindow()),this,SLOT(SltCloseCharWindow()));
    chatWinList.push_back(winChat);
    winChat->AppendMessage(senderNickName,content);
}

void HomePage :: SLtSet(int type,QString signteToSend)
{
    qDebug()<<"进入了修改签名的槽函数";
    QJsonObject jsonObj;
    jsonObj.insert("account",myAccount);
    jsonObj.insert("type",type);
    jsonObj.insert("signate",signteToSend);
    tcp->SltSendMessage(9,jsonObj);
}

void HomePage :: mousePressEvent(QMouseEvent *eve)
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

void HomePage::mouseReleaseEvent(QMouseEvent *eve)
{
    mousePress = false;
}

void HomePage :: mouseMoveEvent(QMouseEvent *eve)
{
    if(mousePress)
    {
        //获取鼠标相对与全屏的位置
        QPoint move_pos = eve->globalPos();
        this->move(move_pos - movePoint);
    }
}

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //test = new windowChat();
    //test->show();
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    isConnected = false;

    //··············label的图片··············
    img = new QImage;
    img->load(":/picture/avatar.jpg");
    ui->labelAvator->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/qqIcon.png");
    ui->labelNum->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/password (.png");
    ui->labelPassword->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/close.png");
    ui->labelClose->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/min.png");
    ui->labelMin->setPixmap(QPixmap::fromImage(*img));
    img->load(":/picture/set.png");
    ui->labelSetting->setPixmap(QPixmap::fromImage(*img));
    img1 = new QImage();
    img2 = new QImage();
    img1->load(":/picture/backgroundTop.jpg");
    ui->labelToppic->setPixmap(QPixmap::fromImage(*img1));
    img2->load(":/picture/registerTop.jpg");

    //初始时隐藏与注册有关的按钮
    ui->pbtRegister->setVisible(false);
    ui->pbtReturn->setVisible(false);
    ui->labelNickName->setVisible(false);
    ui->lineEditNickName->setVisible(false);

    //设置鼠标悬停更改背景颜色
    ui->labelMin->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelSetting->setStyleSheet("QLabel:hover{background-color:rgb(255,255,255,50%);}");
    ui->labelClose->setStyleSheet(":hover{background-color:red;}");
    ui->pbtLogin->setStyleSheet("QPushButton{color:white;background:rgb(31,200,253)}");
    ui->pbtRegister->setStyleSheet("QPushButton{color:white;background:rgb(221,81,69)}");
    ui->pbtReturn->setStyleSheet("QPushButton{color:white;background:rgb(221,81,69)}");

    //鼠标悬停在“忘记密码”和“注册账号”时，触发变手型，
    ui->labelForget->setCursor(QCursor(Qt::PointingHandCursor));
    ui->labelRegister->setCursor(QCursor(Qt::PointingHandCursor));

  //忘记密码与注册账号加入下划线
    QFont f1=ui->labelForget->font();
    f1.setUnderline(true);
    ui->labelForget->setFont(f1);

    //密码不可见
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    QFont f2=ui->labelRegister->font();
    f2.setUnderline(true);
    ui->labelRegister->setFont(f2);

  //label标签安装事件过滤器，使得能在点击后生效
    ui->labelClose->installEventFilter(this);
    ui->labelMin->installEventFilter(this);
    ui->labelSetting->installEventFilter(this);
    ui->labelRegister->installEventFilter(this);
   //测试阶段，打开自动连接本机的服务器，后期修改
    tcp = new clientSocket();


    tcp->ConnectToHost();

    //根据tcp的收到的消息而产生不同信号，从而触发不同的事件
    connect(tcp,SIGNAL(signalStatus(int)),this,SLOT(SltTcpStatus(int)));
}
//    根据服务器返回的消息来确定是否注册或者是登陆成功
void Widget::SltTcpStatus(int type)
{
    qDebug()<<"typeSingalStatus:"<<type;
    switch (type) {
        case 10:
        {
            msgBox.setText("注册成功，请返回至登陆");
            msgBox.exec();
            break;
        }
        case 11:
        {
            msgBox.setText("注册失败，账号冲突");
            msgBox.exec();
            break;
        }
        case 12:
        {
        //根据lineTextNum的消息来判断用户
        //设置登陆后的页面的tcp
        HMPage = new HomePage();
        HMPage -> SetSocket(ui->lineEditNum->text(),tcp->GetUserId());
//        HMPage -> tcpConnectToServer();
        //HMPage -> getFriInfo();
        HMPage->show();
//        关闭登陆窗口
        close();
        break;
        }
        case 13:
        {
            msgBox.setText("输入的账号或密码有误");
            msgBox.exec();
            break;
        }
        case 14:
        {
            msgBox.setText("你重复登陆了");
            msgBox.exec();
            break;
        }
    }
}

//label事件
bool Widget::eventFilter(QObject* object,QEvent *eve)
{
     //最小化窗口
    if(eve->type()==QEvent::MouseButtonPress && object==ui->labelMin)
       showMinimized();

    //关闭窗口
    else if(eve->type()==QEvent::MouseButtonPress && object==ui->labelClose)
        close();

    //点击注册按钮，点击触发注册事件
    else if(eve->type()==QEvent::MouseButtonPress && object==ui->labelRegister)
    {
        mousePress = false;
        //隐藏与登陆有关的组件，只显示与注册有关的组件
        ui->labelToppic->setPixmap(QPixmap::fromImage(*img2));
        ui->checkBoxAuto->setVisible(false);
        ui->checkBoxForget->setVisible(false);
        ui->pbtLogin->setVisible(false);
        ui->labelRegister->setVisible(false);
        ui->labelForget->setVisible(false);
        ui->pbtRegister->setVisible(true);
        ui->pbtReturn->setVisible(true);
        ui->labelNickName->setVisible(true);
        ui->lineEditNickName->setVisible(true);
        ui->lineEditNum->clear();
        ui->lineEditPassword->clear();
    }
    // 尚未开发忘记密码功能
     else if(eve->type() == QEvent::HoverEnter && object == ui->labelForget)
     {
         QFont f1=ui->labelForget->font();
         f1.setUnderline(true);
         ui->labelForget->setFont(f1);
     }
    return false;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pbtReturn_clicked()
{
    //返回至登陆界面，隐藏与注册有关的组件，只显示与登陆有关的组件
    ui->labelToppic->setPixmap(QPixmap::fromImage(*img1));
    ui->checkBoxAuto->setVisible(true);
    ui->checkBoxForget->setVisible(true);
    ui->pbtLogin->setVisible(true);
    ui->labelRegister->setVisible(true);
    ui->labelForget->setVisible(true);
    ui->pbtRegister->setVisible(false);
    ui->pbtReturn->setVisible(false);
    ui->labelNickName->setVisible(false);
    ui->lineEditNickName->setVisible(false);
    ui->lineEditNum->clear();
    ui->lineEditPassword->clear();
}

//点击注册按钮后向服务器端发送消息
void Widget::on_pbtRegister_clicked()
{
    QJsonObject json;
    //首先检查输入是否合法
    QString InputAccount = ui->lineEditNum->text();
    bool isDigtal = InputAccount.contains(QRegExp("^\\d+$"));//判断是否全为数字
    if(isDigtal)
    {
        QString strPassword = ui->lineEditPassword->text();
        bool isDigLet = isLetterNum(strPassword);
        if(isDigLet)
        {
            json.insert("account",ui->lineEditNum->text());
            json.insert("password",ui->lineEditPassword->text());
            json.insert("NickName",ui->lineEditNickName->text());
            //向服务器发送数据，1为发送消息的类型为registe类，json对象储存数据
            //1表示消息为注册
            tcp->SltSendMessage(1,json);
        }
        else {
            msgBox.setText("密码必须全由数字与字母组成");
            msgBox.exec();
            ui->lineEditNum->clear();
            ui->lineEditPassword->clear();
            ui->lineEditNickName->clear();
        }
    }
    else {
        msgBox.setText("账号必须为全数字");
        msgBox.exec();
        ui->lineEditNum->clear();
        ui->lineEditPassword->clear();
        ui->lineEditNickName->clear();
    }
}

void Widget::on_pbtLogin_clicked()
{
    QJsonObject json;

    json.insert("account",ui->lineEditNum->text());
    json.insert("password",ui->lineEditPassword->text());
    //向服务器发送数据，2为发送消息的类型为login类，json对象储存数据

    tcp->SltSendMessage(2,json);//2表示消息类型为登陆请求
}

void Widget :: mousePressEvent(QMouseEvent *eve)
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
void Widget::mouseReleaseEvent(QMouseEvent *eve)
{
    mousePress = false;
}
void Widget :: mouseMoveEvent(QMouseEvent *eve)
{
    if(mousePress)
    {
        //获取鼠标相对与全屏的位置
        QPoint move_pos = eve->globalPos();
        this->move(move_pos - movePoint);
    }
}

bool Widget::isLetterNum(QString str)
{
    QByteArray ba = str.toLatin1();//QString 转换为 char*
    const char* s = ba.data();
    for(int i = 0;i< ba.size();i++)
    {
       if(*s && ((*s>='0' && *s<='9') || (*s>='a' && *s<='z')))
           s++;
       else return false;
    }
    return true;
}

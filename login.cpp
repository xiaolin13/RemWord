#include "login.h"
#include "ui_login.h"

//按钮格式
const QString button_style="QPushButton{background-color:white;\
                                      color: black;   border-radius: 10px;  border: 2px groove gray;\
                                      border-style: outset;}"
                                     "QPushButton:hover{background-color:rgb(60, 220, 255); color: black;}"
                                    "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                                     border-style: inset; }";

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    //初始化背景
    paint();

    //初试化左边控件
    initBackground();

    //消去标题栏
    headdispear();

    //界面显示当前时间
    showtime();
    connect(timer,SIGNAL(timeout()),this,SLOT(slotloginshowtime()));

    //设置按钮样式
    pushbuttonstyle();

    //生成验证码
    createCodeArea();

    showRecord(); //显示登陆记录
    connect(ui->loginusrLineEdit,SIGNAL(currentTextChanged(QString)),this,SLOT(changePass()));

    user=new UserInterface();//子窗口消失后又跳出来
    connect(user,SIGNAL(createWindow()),this,SLOT(slotShowWindow()));

    connect(ui->logincheckLineEdit,SIGNAL(returnPressed()),this,SLOT(on_loginloginPushButton_clicked())); //回车即可登陆
}

login::~login()
{
    delete ui;
}

// timer初试化
void login::showtime() {
    timeisShow=true;
    timer=new QTimer(this);
    timer->start(1000);  //计时开始，间隔一秒
}

 //每秒刷新时间，以“：”区别
void login::slotloginshowtime()
{
    QTime time=QTime::currentTime();
    QString text=time.toString("hh:mm");
    if(timeisShow)
    {
        text[2]=' ';
        timeisShow=false;
    }
    else
    {
        text[2]=':';
        timeisShow=true;
    }
    ui->logintimeLabel->setText(text);
}

//关闭界面
void login::on_loginclosePushButton_clicked()
{
    close();
}

//关闭界面
void login::on_loginexitPushbutton_clicked()
{
    close();
}

void login::on_loginzhucePushButton_clicked()
{
    createregist();//生成注册界面
}

void login::on_loginforgetPushButton_clicked()
{
    createforget(); //生成忘记密码界面
}

void login::on_loginloginPushButton_clicked()
{
    if(!checkIsEmpty()) //信息不全
    {
        ui->loginwarningLabel->setText("<font color=red size=4><b>信息不能为空</b></font>");
        return;
    }
    /*if(!checkCode()) //验证码不对
    {
        ui->loginwarningLabel->setText("<font color=red size=4><b>验证码错误</b></font>");
        ui->logincheckLineEdit->clear(); //将验证码那栏清空
        codeArea->replaceCodePic(); //重置验证码
        update(); //更新
        return;
    }*/
    if(checkUser()) //检查用户存不存在
    {
        ui->loginwarningLabel->setText("<font color=red size=4><b>用户不存在</b></font>");
        ui->loginusrLineEdit->clear();  //清空各栏
        ui->loginpassLineEdit->clear();
        ui->logincheckLineEdit->clear();
        codeArea->replaceCodePic(); //重置验证码
        update(); //更新
        return;
    }
    if(!checkPass()) //检查用户密码
    {
        ui->loginwarningLabel->setText("<font color=red size=4><b>密码错误</b></font>");
        ui->loginpassLineEdit->clear(); //清空一下
        ui->logincheckLineEdit->clear();
        codeArea->replaceCodePic(); //重置验证码
        update(); //更新
        return;
    }
    insertRecord(); //插入登录记录
    createUser(); //生成用户界面
}

void login::on_loginOkPushButton_clicked()
{
    QPixmap pixmap;
    QPalette pa; //账号密码字体一定是黑色
    pa.setColor(QPalette::Text,Qt::black);
    pe.setColor(QPalette::WindowText,Qt::white); //默认界面文本是白色
    pe.setColor(QPalette::Text,Qt::white);
    pe.setColor(QPalette::ButtonText,Qt::white);
    if(ui->loginBackgroundListWidget->currentRow()==0)  //通过所选图片设置背景
    {
        pixmap.load(":/background/1.jpg");  
    }
    else if(ui->loginBackgroundListWidget->currentRow()==1)
    {
        pixmap.load(":/background/2.jpg");
        pe.setColor(QPalette::WindowText,Qt::black); //全局文本颜色
        pe.setColor(QPalette::Text,Qt::black);  //控件文本颜色
        pe.setColor(QPalette::ButtonText,Qt::black); //按钮文本颜色
    }
    else if(ui->loginBackgroundListWidget->currentRow()==2)
    {
        pixmap.load(":/background/3.jpg");
        pe.setColor(QPalette::WindowText,Qt::black);
        pe.setColor(QPalette::Text,Qt::black);
        pe.setColor(QPalette::ButtonText,Qt::black);
    }
    else if(ui->loginBackgroundListWidget->currentRow()==3)
    {
        pixmap.load(":/background/4.jpg");
    }
    else if(ui->loginBackgroundListWidget->currentRow()==4)
    {
        pixmap.load(":/background/5.jpg");
    }
    else if(ui->loginBackgroundListWidget->currentRow()==5)
    {
        pixmap.load(":/background/6.jpg");
    }
    else if(ui->loginBackgroundListWidget->currentRow()==6)
    {
        pixmap.load(":/background/7.jpg");
        pe.setColor(QPalette::WindowText,Qt::black);
    }
    pe.setBrush(this->backgroundRole(),QBrush(pixmap));
    setPalette(pe);
    ui->loginBackgroundListWidget->setPalette(pe);
    ui->loginclosePushButton->setPalette(pe);
    ui->loginbackgroundPushButton->setPalette(pe);
    ui->loginusrLineEdit->setPalette(pa); //各种输入框文本必须是黑色
    ui->loginpassLineEdit->setPalette(pa);
    ui->logincheckLineEdit->setPalette(pa);
    setAutoFillBackground(true);

    //把listwidget隐藏起来
    on_loginReturnPushButton_clicked();
}

/*背景重绘事件 还用不上
void login::paintEvent(QPaintEvent *)
{
   //QPainter paint(this);
   //paint.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/background/2.jpg"));
} */

//初始化界面 （给背景上图）
void login::paint()
{
    QPixmap pixmap(":/background/2.jpg");
    pe.setBrush(this->backgroundRole(),QBrush(pixmap));
    setPalette(pe);
    setAutoFillBackground(true);
    ui->loginpassLineEdit->setEchoMode(QLineEdit::Password);
}

//三种鼠标响应事件，使能拖动无标题栏窗口
void login::mousePressEvent(QMouseEvent *event) //点击
{
    if(event->button()==Qt::LeftButton)
    {
        loginIsdrag=true;
        dragPosition=event->pos();
    }
}

void login::mouseMoveEvent(QMouseEvent *event) //移动
{
    if(loginIsdrag)
    {
        move(event->pos()-dragPosition+pos());
    }
    return QWidget::mouseMoveEvent(event);
}

void login::mouseReleaseEvent(QMouseEvent *) //释放
{
    loginIsdrag=false;
}

//消去标题栏
void login::headdispear()
{
    setWindowFlags(Qt::FramelessWindowHint); //消去标题栏
    setAutoFillBackground(true);
}

//设置按钮样式
void login::pushbuttonstyle()
{
    ui->loginloginPushButton->setCursor(QCursor(Qt::PointingHandCursor)); //图标按钮为一个手
    ui->loginloginPushButton->setStyleSheet(button_style);

    ui->loginzhucePushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginzhucePushButton->setStyleSheet(button_style);

    ui->loginforgetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginforgetPushButton->setStyleSheet(button_style);

    ui->loginexitPushbutton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginexitPushbutton->setStyleSheet(button_style);

    ui->loginclosePushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginclosePushButton->setFlat(true);
    ui->loginclosePushButton->setStyleSheet("QPushButton:hover{border-image:url(:/pic/logo.png);color:red}");

    ui->loginbackgroundPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginbackgroundPushButton->setFlat(true);
    ui->loginbackgroundPushButton->setStyleSheet("QPushButton:hover{border-image:url(:/pic/logo.png);color:blue}");

    ui->loginReturnPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginReturnPushButton->setFlat(true);
    ui->loginReturnPushButton->setStyleSheet(button_style);

    ui->loginOkPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginOkPushButton->setFlat(true);
    ui->loginOkPushButton->setStyleSheet(button_style);

    ui->loginChangeButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginChangeButton->setFlat(true);
    ui->loginChangeButton->setStyleSheet("QPushButton:hover{border-image:url(:/pic/logo.png);color:blue}");

}

//初试化验证码函数
void login::createCodeArea()
{
    codeArea=new CodeArea(this);
    ui->codeLayout->addWidget(codeArea);
}

//显示修改背景图片
void login::on_loginbackgroundPushButton_clicked()
{
    ui->loginBackgroundListWidget->show();
    ui->loginReturnPushButton->show();
    ui->loginOkPushButton->show();
}

//隐藏修改背景图片
void login::on_loginReturnPushButton_clicked()
{
    ui->loginBackgroundListWidget->hide();
    ui->loginReturnPushButton->hide();
    ui->loginOkPushButton->hide();
}

//初始化修改背景的按钮
void login::initBackground()
{
    ui->loginBackgroundListWidget->setStyleSheet("background-color:transparent"); //设置listwidget背景透明
    ui->loginBackgroundListWidget->hide(); //一开始listwidget和按钮是隐藏的
    ui->loginReturnPushButton->hide();
    ui->loginOkPushButton->hide();
    QListWidgetItem *item =ui->loginBackgroundListWidget->item(0);
    item->setSelected(true);
}

//生成注册界面
void login::createregist()
{
    regist=new RegistAccount();
    regist->show();
}

//生成忘记密码界面
void login::createforget()
{
    forget=new ForgetPass();
    forget->show();
}

void login::createUser()  //生成用户界面
{
    user->drawbackground(pe);
    user->userinterName=ui->loginusrLineEdit->currentText(); //将用户名传递过去
    user->createName(); //将用户名放上去
    user->setPicture();
    user->setData();
    user->Shengchengdanci();
    user->show();
    this->hide(); //隐藏主窗口
}

void login::slotShowWindow() //主窗口再现
{
    ui->loginusrLineEdit->clear();
    ui->loginpassLineEdit->clear();
    codeArea->replaceCodePic();
    update();
    ui->logincheckLineEdit->clear();
    showRecord();
    this->show();
}

bool login::checkIsEmpty() //检查是否有信息为空
{
    if(ui->loginusrLineEdit->currentText().isEmpty()||ui->loginpassLineEdit->text().isEmpty()||ui->logincheckLineEdit->text().isEmpty())
        return false;
    return true;
}

bool login::checkUser() //检查用户是否存在
{
    bool is=true;
    QSqlDatabase logintdb=QSqlDatabase::addDatabase("QSQLITE"); //设置数据库类型为sqlite
    logintdb.setDatabaseName(".\\database\\userInfo.db");
    logintdb.open();
    QSqlQuery query;
    query.prepare("select * from users where userName=:d");
    query.bindValue(":d",ui->loginusrLineEdit->currentText());
    query.exec();
    if(query.next())  //判断有没有该用户
    {
       loginPass=query.value(1).toString();
       is=false;
    }
    logintdb.close();
    return is;
}

bool login::checkPass() //检查密码对不对
{
    if(ui->loginpassLineEdit->text()!=loginPass)  //loginPass为搜寻得到的密码
        return false;
    return true;
}

bool login::checkCode() //检查验证码对不对
{
    if(ui->logincheckLineEdit->text()!=codeArea->m_sCode)  //codeArea->m_sCode 为此验证码对应的文本
        return false;
    return true;
}

void login::on_loginChangeButton_clicked() //更换验证码
{
    codeArea->replaceCodePic();
    update();
}

void login::insertRecord() //插入登录记录
{
    if(ui->loginCheckBox->checkState()==Qt::Checked) //根据是否记住密码做调整
        mima=ui->loginpassLineEdit->text();
    else mima="";
    QSqlDatabase recorddb=QSqlDatabase::addDatabase("QSQLITE");
    recorddb.setDatabaseName(".\\database\\userInfo.db");
    recorddb.open();
    QSqlQuery query;
    query.prepare("delete from usersrecord where userName=:d");  //先删后插
    query.bindValue(":d",ui->loginusrLineEdit->currentText());
    query.exec();
    query.prepare("insert into usersrecord values(NULL,:d1,:d2)");
    query.bindValue(":d1",ui->loginusrLineEdit->currentText());
    query.bindValue(":d2",mima);
    query.exec();
    recorddb.close();
}

void login::showRecord() //显示登陆记录
{
    QSqlDatabase logindb=QSqlDatabase::addDatabase("QSQLITE");
    logindb.setDatabaseName(".\\database\\userInfo.db");
    logindb.open();
    QSqlQuery query;
    query.exec("select * from usersrecord order by id desc limit 3");
    query.next();
    ui->loginusrLineEdit->addItem(query.value(1).toString());
    ui->loginpassLineEdit->setText(query.value(2).toString());
    while(query.next())
    {
        ui->loginusrLineEdit->addItem(query.value(1).toString());
    }
    logindb.close();
}

void login::changePass() //响应用户名改变事件
{
    ui->loginpassLineEdit->clear();
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(".\\database\\userInfo.db");
    db.open();
    QSqlQuery query;
    query.prepare("select * from usersrecord where userName=:d");
    query.bindValue(":d",ui->loginusrLineEdit->currentText());
    query.exec();
    if(query.next())
        ui->loginpassLineEdit->setText(query.value(2).toString());
    db.close();
}

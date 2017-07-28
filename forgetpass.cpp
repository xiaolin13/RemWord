#include "forgetpass.h"
#include "ui_forgetpass.h"

//按钮格式
const QString button_style="QPushButton{background-color:white;\
                                      color: black;   border-radius: 10px;  border: 2px groove gray;\
                                      border-style: outset;}"
                                     "QPushButton:hover{background-color:rgb(60, 220, 255); color: black;}"
                                    "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                                     border-style: inset; }";

ForgetPass::ForgetPass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForgetPass)
{
    ui->setupUi(this);

    headDisappear();//绘制背景和隐藏标题栏
    forgetButton(); //按钮样式

    //connect(this,SIGNAL(closeWindow()),this,SLOT(WindowClose())); //关闭窗口响应 （还实现不了） gg
}

ForgetPass::~ForgetPass()
{
    delete ui;
}

//三种鼠标响应事件，使能拖动无标题栏窗口
void ForgetPass::mousePressEvent(QMouseEvent *event) //点击
{
    if(event->button()==Qt::LeftButton)
    {
        forgetIsdrag=true;
        forgetDragpoint=event->pos();
    }
}

void ForgetPass::mouseMoveEvent(QMouseEvent *event) //移动
{
    if(forgetIsdrag)
    {
        move(event->pos()-forgetDragpoint+pos());
    }
    return QWidget::mouseMoveEvent(event);
}

void ForgetPass::mouseReleaseEvent(QMouseEvent *) //释放
{
    forgetIsdrag=false;  //做判断
}

//消去标题栏 右上角文本改为白色
void ForgetPass::headDisappear()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    QPalette forgetpa;
    forgetpa.setColor(QPalette::ButtonText,Qt::white);
    ui->forgetExitButton->setPalette(forgetpa);
}

//背景重绘
void ForgetPass::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/background/forget.jpg"));
}

void ForgetPass::on_forgetExitButton_clicked()
{
    close();
}

void ForgetPass::on_forgetNoButton_clicked()
{
    close();
}

//按钮样式
void ForgetPass::forgetButton()
{
    ui->forgetOkButton->setCursor(QCursor(Qt::PointingHandCursor));//确认按钮
    ui->forgetOkButton->setStyleSheet(button_style);

    ui->forgetExitButton->setCursor(QCursor(Qt::PointingHandCursor)); //返回按钮
    ui->forgetNoButton->setStyleSheet(button_style);

    ui->forgetExitButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->forgetExitButton->setFlat(true); //右上角关闭按钮
    ui->forgetExitButton->setStyleSheet("QPushButton:hover{border-image:url(:/pic/logo.png);color:red}");
}

void ForgetPass::on_forgetOkButton_clicked()
{
    if(!checkIsEmpty())  //存在信息为空
    {
         ui->ForgetWarnLabel->setText("<font color=red size=4><b>信息不能为空</b></font>");
         return;
    }
    if(checkUExist())  //检查用户是否为空
    {
        ui->ForgetWarnLabel->setText("<font color=red size=4><b>用户不存在</b></font>");
        ui->forgetNLabel->clear();
        ui->forgetELineEdit->clear();
        return;
    }
    if(!checkEIsRight())  //检查邮箱是否正确
    {
        ui->ForgetWarnLabel->setText("<font color=red size=4><b>邮箱错误</b></font>");
        ui->forgetELineEdit->clear();
        return;
    }
    QPalette pa;
    pa.setColor(QPalette::Text,Qt::white);
    ui->ForgetWarnLabel->setPalette(pa);
    ui->ForgetWarnLabel->setText(tr("密码是:")+userPass); //将密码告诉用户

    emit closeWindow(); //通知窗口要关闭了
}

bool ForgetPass::checkUExist()  //检查用户是否存在
{
    bool is=true;
    QSqlDatabase forgetdb=QSqlDatabase::addDatabase("QSQLITE"); //设置数据库类型为sqlite
    forgetdb.setDatabaseName(".\\database\\userInfo.db");
    forgetdb.open();
    QSqlQuery query;
    query.prepare("select * from users where userName=:d"); //搜寻数据库是否已经有该名字出现
    query.bindValue(":d",ui->forgetNLineEdit->text());
    query.exec();
    if(query.next())
    {
        is=false;
    }
    forgetdb.close();
    return is;
}

bool ForgetPass::checkEIsRight()  //检查该用户的邮箱是否正确
{
    QSqlDatabase forgetdb=QSqlDatabase::addDatabase("QSQLITE"); //设置数据库类型为sqlite
    forgetdb.setDatabaseName(".\\database\\userInfo.db");
    forgetdb.open();
    QSqlQuery query;
    query.prepare("select * from users where userName=:d");
    query.bindValue(":d",ui->forgetNLineEdit->text());
    query.exec();
    query.next();
    QString forgetEmail=query.value(2).toString();     //获取邮箱
    userPass=query.value(1).toString();
    forgetdb.close();
    if(forgetEmail==ui->forgetELineEdit->text()) //如果相等返回true
        return true;
    return false;
}

bool ForgetPass::checkIsEmpty() //检查是否有信息未填
{
    if(ui->forgetELineEdit->text().isEmpty()||ui->forgetNLineEdit->text().isEmpty()) //有信息为空时
        return false;
    return true;
}

void ForgetPass::WindowClose() //响应关闭窗口信号
{
    time.start();
    while(time.elapsed()<2000);
    close();
}

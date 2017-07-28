#include "registaccount.h"
#include "ui_registaccount.h"

//按钮格式
const QString button_style="QPushButton{background-color:white;\
                                      color: black;   border-radius: 10px;  border: 2px groove gray;\
                                      border-style: outset;}"
                                     "QPushButton:hover{background-color:rgb(60, 220, 255); color: black;}"
                                    "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                                     border-style: inset; }";


RegistAccount::RegistAccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistAccount)
{
    ui->setupUi(this);

    //隐藏标题栏
    headDisappear();

    //按钮样式
    registpushstyle();
}

RegistAccount::~RegistAccount()
{
    delete ui;
}

void RegistAccount::on_registClostButton_clicked()
{
    close();
}

void RegistAccount::on_registReturnButton_clicked()
{
    close();
}

//三种鼠标响应事件，使能拖动无标题栏窗口
void RegistAccount::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        registIsdrag=true;
        registDragpoint=event->pos();
    }
}

void RegistAccount::mouseMoveEvent(QMouseEvent *event)
{
    if(registIsdrag)
    {
        move(event->pos()-registDragpoint+pos());
    }
    return QWidget::mouseMoveEvent(event);
}

void RegistAccount::mouseReleaseEvent(QMouseEvent *)
{
    registIsdrag=false;  //做判断
}

//消去标题栏 右上角关闭按钮文本变成白色 设置邮箱格式
void RegistAccount::headDisappear()
{
    setWindowFlags(Qt::FramelessWindowHint);
    ui->registStackwidget->setCurrentIndex(0); //将stackwidget窗口  默认为0
    QPixmap pixmap(":/background/空.jpg"); //
    ui->registButton->setIcon(pixmap); //设置默认头像
    setAutoFillBackground(true);
    QPalette registpa;
    registpa.setColor(QPalette::ButtonText,Qt::gray);
    ui->registClostButton->setPalette(registpa);
    QRegExp regexp("[1-9][0-9]{8,9}[@][q][q][//.][c][o][m]");  //固定格式为8-9位的qq邮箱格式
    ui->registEmailLineEdit->setValidator(new QRegExpValidator(regexp,this)); //将改正则表达式应用上去
}

//按钮样式
void RegistAccount::registpushstyle()
{
    ui->registOkbutton->setStyleSheet(button_style); //确定按钮
    ui->registOkbutton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->registButton2->setCursor(QCursor(Qt::PointingHandCursor)); //修改头像按钮
    ui->registButton2->setStyleSheet(button_style);

    ui->registReturnButton->setStyleSheet(button_style); //返回按钮
    ui->registReturnButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->registClostButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->registClostButton->setFlat(true); //右上角关闭按钮
    ui->registClostButton->setStyleSheet("QPushButton:hover{border-image:url(:/pic/logo.png);color:red}");
}

//背景重绘
void RegistAccount::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/background/regist.jpg"));
}


//数据库检索，并判断输入数据是否符合规范
void RegistAccount::on_registOkbutton_clicked()
{
    if(!hasempty()) //看是否有数据为空
    {
        ui->registWarnLabel->setText("<font color=red size=4><b>信息不能为空</b></font>"); //设置字体格式
        return;
    }
    if(!passDifferent()) //密码是否一致
    {
        ui->registWarnLabel->setText("<font color=red size=4><b>密码不一致</b></font>");
        ui->registNPassLineEdit->clear();
        ui->registPassLineEdit->clear();
        return;
    }
    if(!checkEmailStyle())
    {
        ui->registEmailLineEdit->clear();
        return;
    }
    if(!repeatName()) //名字是否出现重复
    {
        ui->registWarnLabel->setText("<font color=red size=4><b>用户已存在</b></font>");
        ui->registNPassLineEdit->clear(); //清空窗口
        ui->registPassLineEdit->clear();
        ui->registNameLineEdit->clear();
        ui->registEmailLineEdit->clear();
        return;
    }
    insertData(); //插入用户数据
    //closeWindow();
    ui->registStackwidget->setCurrentIndex(1);
}

bool RegistAccount::hasempty()  //看是否有数据为空
{
    if(ui->registNameLineEdit->text().isEmpty()||ui->registPassLineEdit->text().isEmpty()
            ||ui->registNPassLineEdit->text().isEmpty()
            ||ui->registEmailLineEdit->text().isEmpty())
        return false;
    return true;
}

bool RegistAccount::passDifferent()  //密码是否一致
{
    if(ui->registPassLineEdit->text()!=ui->registNPassLineEdit->text())
        return false;
    return true;
}

bool RegistAccount::repeatName()  //名字是否出现重复
{
    bool is=true;
    QSqlDatabase registdb=QSqlDatabase::addDatabase("QSQLITE"); //设置数据库类型为sqlite
    registdb.setDatabaseName(".\\database\\userInfo.db");
    registdb.open();
    QSqlQuery query;
    query.prepare("select * from users where userName=:d"); //搜寻数据库是否已经有该名字出现
    query.bindValue(":d",ui->registNameLineEdit->text());
    query.exec();
    if(query.next())
    {
        is=false;
    }
    registdb.close();
    return is;
}

void RegistAccount::insertData() //插入用户数据
{
    QSqlDatabase registdb=QSqlDatabase::addDatabase("QSQLITE"); //设置数据库类型为sqlite
    registdb.setDatabaseName(".\\database\\userInfo.db");
    registdb.open();
    QSqlQuery query;
    query.prepare("insert into users values(:d1,:d2,:d3)");  //占位，然后变量插入
    query.bindValue(":d1",ui->registNameLineEdit->text());
    query.bindValue(":d2",ui->registPassLineEdit->text());
    query.bindValue(":d3",ui->registEmailLineEdit->text());
    query.exec();
    registdb.close();
}

void RegistAccount::closeWindow()  //先清屏，两秒后关闭窗口(实现不了)
{
    ui->registNPassLineEdit->clear(); //清空窗口,完成注册成功后的操作
    ui->registPassLineEdit->clear();
    ui->registNameLineEdit->clear();
    ui->registEmailLineEdit->clear();
    QMessageBox::information(this,tr("successful"),tr("注册成功，返回登陆"));
    close();
}

bool RegistAccount::checkEmailStyle() //检查邮箱格式
{
    if(ui->registEmailLineEdit->text().length()<16)  //长度太短
    {
        ui->registWarnLabel->setText("<font color=red size=4><b>邮箱格式不对</b></font>");
        return false;
    }
    if(ui->registEmailLineEdit->text().length()==16)
    {
        if(ui->registEmailLineEdit->text().at(15)!='m')  //看九位数时对不对
        {
            ui->registWarnLabel->setText("<font color=red size=4><b>邮箱格式不对</b></font>");
            return false;
        }
    }
    return true;
}

void RegistAccount::on_registButton2_clicked()
{
    close();
}

void RegistAccount::on_registButton_clicked() //选择头像
{
    QString fangdastr=".\\picture\\"+ui->registNameLineEdit->text()+".jpg";  //按指定jpg格式打开图片
    QString fangdafiles = QFileDialog::getOpenFileName(this,
                tr("da打开图片"),
                QDir::currentPath(),
                "Images (*.jpg)",
                0);
    if(fangdafiles.isEmpty())
    {
            return;
    }
    QFile file(fangdastr);
    if(file.exists())
    {
        file.remove();
     }
     QImage fangdaimage;
     fangdaimage.load(fangdafiles);
     ui->registButton->setIcon(QPixmap::fromImage(fangdaimage));
     fangdaimage.save(fangdastr);
     update();
}

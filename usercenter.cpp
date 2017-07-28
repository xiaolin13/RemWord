#include "usercenter.h"
#include "ui_usercenter.h"

const QString button_style="QPushButton{background-color:white;\
                                      color: black;   border-radius: 10px;  border: 2px groove gray;\
                                      border-style: outset;}"
                                     "QPushButton:hover{background-color:rgb(60, 220, 255); color: black;}"
                                    "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                                     border-style: inset; }";

UserCenter::UserCenter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserCenter)
{
    ui->setupUi(this);

    centreHead(); //消去标题栏，改变文本颜色为白色 listwidget 背景为透明
    ButtonStyle(); //改变按钮样式
    //将listwidget 与 stackwidget 关联起来
    QObject::connect(ui->CenterListWidget,SIGNAL(currentRowChanged(int)),ui->CentreStackWidget,SLOT(setCurrentIndex(int)));
}

UserCenter::~UserCenter()
{
    delete ui;


}

//三种鼠标响应事件，使能拖动无标题栏窗口
void UserCenter::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        centreIsdrag=true;
        centrePoint=event->pos();
    }
}

void UserCenter::mouseMoveEvent(QMouseEvent *event)
{
    if(centreIsdrag)
    {
        move(event->pos()-centrePoint+pos());
    }
    return QWidget::mouseMoveEvent(event);
}

void UserCenter::mouseReleaseEvent(QMouseEvent *)
{
    centreIsdrag=false;  //做判断
}

void UserCenter::paintEvent(QPaintEvent *) //背景重绘事件
{
    QPainter paint(this);
    paint.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/background/centre.jpg"));
}
//消去标题栏，改变文本颜色为白色 listwidget 背景为透明 设置填写邮箱格式
void UserCenter::centreHead()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    ui ->CenterListWidget->setStyleSheet("background-color:transparent");  //listwidget背景为透明
    QRegExp regexp("[1-9][0-9]{8,9}[@][q][q][//.][c][o][m]");  //固定格式为8-9位的qq邮箱格式
    ui->CentreOEMLineEdit->setValidator(new QRegExpValidator(regexp,this)); //将改正则表达式应用上去
    ui->CentreNEMLineEdit->setValidator(new QRegExpValidator(regexp,this));
    ui->CentreNEMQLineEdit->setValidator(new QRegExpValidator(regexp,this));
}

void UserCenter::on_CentreNoButton_clicked()
{
    close();
}

void UserCenter::ButtonStyle()  //按钮样式修改
{
    ui->CentreEButton->setCursor(QCursor(Qt::PointingHandCursor)); //修改邮箱按钮
    ui->CentreEButton->setStyleSheet(button_style);

    ui->CentreEButton->setCursor(QCursor(Qt::PointingHandCursor)); //修改邮箱按钮
    ui->CentreEButton->setStyleSheet(button_style);

    ui->CentrePOkButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->CentrePOkButton->setStyleSheet(button_style);

    ui->CentreNoButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->CentreNoButton->setFlat(true);
    ui->CentreNoButton->setStyleSheet("QPushButton:hover{border-image:url(:/pic/logo.png);color:red}");
}

void UserCenter::createCenterName() //生成用户名
{
    ui->CentreNameLabel->setText(UserCenterName);
}

void UserCenter::on_CentrePOkButton_clicked() //修改密码
{
    if(!checkIsEmpty()) //信息是否为全
    {
        ui->CentreEWarning->setText("<font color=red size=4><b>信息不全</b></font>");
        return;
    }
    if(!PassDifferent()) //前后密码是否一致
    {
        ui->CentreEWarning->setText("<font color=red size=4><b>密码不一致</b></font>");
        ui->CentreNPLineEdit->clear();
        ui->CentreNPQLineEdit->clear();
        return;
    }
    if(!checkPass()) //检查密码是否一致
    {
        ui->CentreEWarning->setText("<font color=red size=4><b>密码错误</b></font>");
        ui->CentreOPLineEdit->clear();
        ui->CentreNPQLineEdit->clear();
        ui->CentreNPLineEdit->clear();
        return;
    }
    updatePass();  //更新数据
    QMessageBox::information(this,tr("success"),tr("修改密码成功，新密码会在下次登陆生效"));
    close();
}

bool UserCenter::checkPass() //检查密码
{
    QSqlDatabase centerdb=QSqlDatabase::addDatabase("QSQLITE");
    centerdb.setDatabaseName(".\\database\\userInfo.db");
    centerdb.open();
    QSqlQuery query;
    query.prepare("select userPass from users where userName=:d"); //搜索用户
    query.bindValue(":d",UserCenterName);
    query.exec();
    if(query.next())
    {
        centerPass=query.value(0).toString();
    }
    centerdb.close();
    if(centerPass!=ui->CentreOPLineEdit->text())
        return false;
    return true;
}

bool UserCenter::checkIsEmpty()  //检查是否信息不全
{
    if(ui->CentreOPLineEdit->text().isEmpty()||ui->CentreNPLineEdit->text().isEmpty()||ui->CentreNPQLineEdit->text().isEmpty())
        return false;
    return true;
}

bool UserCenter::PassDifferent() //检查密码是否一致
{
    if(ui->CentreNPLineEdit->text()!=ui->CentreNPQLineEdit->text())
        return false;
    return true;
}

void UserCenter::updatePass() //更新密码
{
    QSqlDatabase centerdb=QSqlDatabase::addDatabase("QSQLITE");
    centerdb.setDatabaseName(".\\database\\userInfo.db");
    centerdb.open();
    QSqlQuery query;
    query.prepare("update users set userPass=:d where userName=:d1"); //更新数据
    query.bindValue(":d",ui->CentreNPLineEdit->text());
    query.bindValue(":d1",UserCenterName);
    query.exec();
    centerdb.close();
}

bool UserCenter::checkEmail() //检查邮箱
{
    QSqlDatabase centerdb=QSqlDatabase::addDatabase("QSQLITE");
    centerdb.setDatabaseName(".\\database\\userInfo.db");
    centerdb.open();
    QSqlQuery query;
    query.prepare("select userEmail from users where userName=:d"); //搜索用户
    query.bindValue(":d",UserCenterName);
    query.exec();
    if(query.next())
    {
        centerEmail=query.value(0).toString();
    }
    centerdb.close();
    if(centerEmail!=ui->CentreOEMLineEdit->text())
        return false;
    return true;
}

bool UserCenter::checkEIsEmpty() //检查是否信息不全
{
    if(ui->CentreOEMLineEdit->text().isEmpty()||ui->CentreNEMLineEdit->text().isEmpty()||ui->CentreNEMQLineEdit->text().isEmpty())
        return false;
    return true;
}

bool UserCenter::EmailDifferent() //检查邮箱是否一致
{
    if(ui->CentreNEMLineEdit->text()!=ui->CentreNEMQLineEdit->text())
        return false;
    return true;
}

void UserCenter::updateEmail() //修改邮箱
{
    QSqlDatabase centerdb=QSqlDatabase::addDatabase("QSQLITE");
    centerdb.setDatabaseName(".\\database\\userInfo.db");
    centerdb.open();
    QSqlQuery query;
    query.prepare("update users set userEmail=:d where userName=:d1"); //更新数据
    query.bindValue(":d",ui->CentreNEMLineEdit->text());
    query.bindValue(":d1",UserCenterName);
    query.exec();
    centerdb.close();
}

void UserCenter::on_CentreEButton_clicked() //修改邮箱
{
    if(!checkEIsEmpty()) //信息是否为全
    {
        ui->CentreEWarnLabel->setText("<font color=red size=4><b>信息不全</b></font>");
        return;
    }
    if(!EmailDifferent()) //前后邮箱是否一致
    {
        ui->CentreEWarnLabel->setText("<font color=red size=4><b>邮箱不一致</b></font>");
        ui->CentreNEMLineEdit->clear();
        ui->CentreNEMQLineEdit->clear();
        return;
    }
    if(!checkEmail()) //检查邮箱是否存在
    {
        ui->CentreEWarnLabel->setText("<font color=red size=4><b>邮箱错误</b></font>");
        ui->CentreOEMLineEdit->clear();
        ui->CentreNEMLineEdit->clear();
        ui->CentreNEMQLineEdit->clear();
        return;
    }
    updateEmail();  //更新数据
    QMessageBox::information(this,tr("success"),tr("修改邮箱成功，新邮箱会在下次登陆生效"));
    close();
}

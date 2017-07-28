#ifndef USERCENTER_H
#define USERCENTER_H

#include <QDialog>
#include <QMouseEvent>
#include <QPainter>

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QMessageBox>

namespace Ui {
class UserCenter;
}

class UserCenter : public QDialog
{
    Q_OBJECT

public:
    explicit UserCenter(QWidget *parent = 0);
    ~UserCenter();

    virtual void mousePressEvent(QMouseEvent *); //响应鼠标点击事件
    virtual void mouseMoveEvent(QMouseEvent *);   //响应鼠标移动事件
    virtual void mouseReleaseEvent(QMouseEvent *);  //响应鼠标释放事件

    virtual void paintEvent(QPaintEvent*);  //背景重绘事件

    QString UserCenterName; //存储用户名字
    void createCenterName(); //生成用户名字

private slots:
    void on_CentreNoButton_clicked();

    void on_CentrePOkButton_clicked();

    void on_CentreEButton_clicked();

private:
    Ui::UserCenter *ui;

    //响应鼠标事件
    bool centreIsdrag;
    QPoint centrePoint;

    //消去标题栏，改变文本颜色 listwidget 背景为透明
    void centreHead();

    //改变鼠标样式
    void ButtonStyle();

    bool checkPass(); //检查密码
    void updatePass(); //修改密码
    QString centerPass; //用来存储密码
    bool checkIsEmpty(); //检查是否信息不全
    bool PassDifferent(); //检查密码是否一致

    bool checkEmail(); //检查邮箱
    void updateEmail(); //修改邮箱
    QString centerEmail; //用来存储邮箱
    bool checkEIsEmpty(); //检查是否信息不全
    bool EmailDifferent(); //检查邮箱是否一致
};

#endif // USERCENTER_H

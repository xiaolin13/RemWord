#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

#include <QTimer>
#include <QTime>

#include <QPixmap>
#include <QPainter>

#include <QMouseEvent>
#include "codearea.h"  //验证码
#include "registaccount.h" //注册账号
#include "forgetpass.h" //忘记密码
#include "userinterface.h" //用户界面

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();
    UserInterface *user; //用户界面的生成

protected:
    /*void paintEvent(QPaintEvent *);  //重绘背景*/ //暂时用不上
    void paint(); //修改背景

    virtual void mousePressEvent(QMouseEvent *); //响应鼠标点击事件
    virtual void mouseMoveEvent(QMouseEvent *);   //响应鼠标移动事件
    virtual void mouseReleaseEvent(QMouseEvent *);  //响应鼠标释放事件

private slots:
    void slotloginshowtime();  //当前时间槽
    void slotShowWindow(); //主窗口再现

    void on_loginclosePushButton_clicked();//关闭函数
    void on_loginexitPushbutton_clicked();//关闭函数

    void on_loginbackgroundPushButton_clicked();

    void on_loginReturnPushButton_clicked();

    void on_loginOkPushButton_clicked();

    void on_loginzhucePushButton_clicked();

    void on_loginforgetPushButton_clicked();

    void on_loginloginPushButton_clicked();

    void on_loginChangeButton_clicked();

    void changePass(); //当用户名改变时，区寻找记录

private:
    Ui::login *ui;

    RegistAccount *regist; //注册界面的生成
    void createregist();

    ForgetPass *forget; //忘记密码界面的生成
    void createforget();

    void createUser(); //用户界面的生成

     QPalette pe; //背景 //button 的文本颜色 //listwidget 的文本颜色

    //界面右下角时间设定
    void showtime();
    QTimer *timer;
    bool timeisShow;

    //消去标题栏，设置窗体可拖动
    void headdispear();
    QPoint dragPosition;
    bool loginIsdrag;

    //设置按钮样式
    void pushbuttonstyle();

    //设置验证码
    CodeArea *codeArea;
    void createCodeArea();

    //初始化修改背景的按钮
    void initBackground();

    bool checkIsEmpty(); //检查是否有信息为空
    bool checkUser(); //检查用户是否存在
    bool checkPass(); //检查密码对不对
    bool checkCode(); //检查验证码对不对

    void insertRecord(); //插入登录记录
    QString mima;//根据是否记住密码而存

    void showRecord(); //comboBox显示登陆信息

    QString loginPass;
};

#endif // LOGIN_H

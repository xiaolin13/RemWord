#ifndef FORGETPASS_H
#define FORGETPASS_H

#include <QDialog>
#include <QMouseEvent>
#include <QPainter>

#include <QtSql>  //数据库相关操作
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QTime>

namespace Ui {
class ForgetPass;
}

class ForgetPass : public QDialog
{
    Q_OBJECT

public:
    explicit ForgetPass(QWidget *parent = 0);
    ~ForgetPass();
    virtual void mousePressEvent(QMouseEvent *); //响应鼠标点击事件
    virtual void mouseMoveEvent(QMouseEvent *);   //响应鼠标移动事件
    virtual void mouseReleaseEvent(QMouseEvent *);  //响应鼠标释放事件

    virtual void paintEvent(QPaintEvent*);  //背景重绘事件

private slots:
    void on_forgetExitButton_clicked();

    void on_forgetNoButton_clicked();

    void on_forgetOkButton_clicked();

    void WindowClose(); //关闭窗口

signals:
    void closeWindow();

private:
    Ui::ForgetPass *ui;

    //消去标题栏
    void headDisappear();
    void forgetButton(); //按钮样式

    //鼠标响应事件
    bool forgetIsdrag;
    QPoint forgetDragpoint;

    bool checkUExist(); //检查用户是否存在
    bool checkEIsRight(); //检查邮箱是否正确
    bool checkIsEmpty(); //检查信息是否为空

    QString userPass; //用来存放用户密码
    QTime time; //计时用
};

#endif // FORGETPASS_H

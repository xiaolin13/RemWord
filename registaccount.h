#ifndef REGISTACCOUNT_H
#define REGISTACCOUNT_H

#include <QDialog>
#include <QMouseEvent> //响应鼠标事件
#include <QPainter> //背景重绘事件
#include <QRegExp> //正则表达式头文件

#include <QSqlQuery> //数据库头文件
#include <QtSql>
#include <QSqlDatabase>

#include <QTime> //两秒后关闭窗口

#include <QMessageBox>
#include <QFileDialog>

namespace Ui {
class RegistAccount;
}

class RegistAccount : public QDialog
{
    Q_OBJECT

public:
    explicit RegistAccount(QWidget *parent = 0);
    ~RegistAccount();

    virtual void mousePressEvent(QMouseEvent *); //响应鼠标点击事件
    virtual void mouseMoveEvent(QMouseEvent *);   //响应鼠标移动事件
    virtual void mouseReleaseEvent(QMouseEvent *);  //响应鼠标释放事件

    virtual void paintEvent(QPaintEvent*);  //背景重绘事件

private slots:
    void on_registClostButton_clicked();

    void on_registReturnButton_clicked();

    void on_registOkbutton_clicked();

    void on_registButton2_clicked();

    void on_registButton_clicked();

private:
    Ui::RegistAccount *ui;

    void headDisappear(); //界面设置无标题栏，可拖动，右上角关闭按钮文本变成白色 设置邮箱格式
    bool registIsdrag; //与能否拖动界面有关
    QPoint registDragpoint;
    void registpushstyle(); //按钮样式
    bool hasempty(); //信息是否有为空
    bool repeatName(); //名字是否已经存在
    bool passDifferent();  //前后密码是否一致
    void insertData(); //插入用户数据
    bool checkEmailStyle(); //检查邮箱格式
    QTime time; //计时用
    void closeWindow(); //先清屏，两秒后关闭窗口
};

#endif // REGISTACCOUNT_H

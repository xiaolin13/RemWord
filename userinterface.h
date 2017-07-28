#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QDialog>
#include <QMouseEvent>//鼠标响应事件

#include <QtNetwork> //网络事件响应
#include <QNetworkReply>
#include <QNetworkRequest>

#include "usercenter.h" //个人中心界面
#include "blowup.h" //放大头像界面

#include <QVector> //容器

#include <QTime> //为种随机数准备

#include <QtDebug> //为debug准备

#include <QMediaPlayer> //播放设备
#include <QFile> //为判断文件是否存在

#include "soundhttp.h" //网上下载音频

namespace Ui {
class UserInterface;
}

class UserInterface : public QDialog
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = 0);
    ~UserInterface();

    virtual void mousePressEvent(QMouseEvent *); //响应鼠标点击事件
    virtual void mouseMoveEvent(QMouseEvent *);   //响应鼠标移动事件
    virtual void mouseReleaseEvent(QMouseEvent *);  //响应鼠标释放事件

    void drawbackground(QPalette &palette);

    QString userinterName; //存储用户名字
    void createName();  //生成用户名字
    void setPicture(); //生成头像
    void setData(); //生成单词数据
    void Shengchengdanci(); //生成单词

private slots:
    void on_userExitButton_clicked();

    void on_userCloseButton_clicked();

    void changeWidget(int i); //窗口切换

    void finishedSlot(QNetworkReply *reply); //网络响应结束后

    void buttonSlot(); //lineEdit为空时不能翻译

    void on_userTButton_clicked();

    void on_userCenterButton_clicked();

    void on_pictureButton_clicked();

    void refreshPicture(); //刷新头像

    void setWordData(); //放置单词数据到界面上

    void on_WordButton1_clicked();

    void on_WordButton2_clicked();

    void on_WordButton3_clicked();

    void on_WordButton4_clicked();

    void on_UsrCollectButton_clicked();

    void on_userTRButton_clicked();

    void on_InterButton_clicked();

    void on_LocalButton_clicked();

    void on_pushButton_clicked();

    void on_voicepushButton_clicked();

    void on_qmediaButton1_clicked();

    void on_userTILineEdit_textChanged(const QString &arg1);

signals:
    void createWindow();

private:
    Ui::UserInterface *ui;

    QNetworkReply *reply; //网络回应

    QMediaPlayer *player; //媒体播放器

    //响应鼠标事件
    bool uiIsdrag;
    QPoint uiPointDrag;

    void dishb(); //消去标题栏
    void userButtonstyle(); //列表背景透明 按钮样式

    UserCenter *useC;  //生成个人中心界面
    void createCenter();

    blowup *blow; //生成放大头像界面

    QVector<QVector<QString>> wordInformation; //用来保存所有单词信息
    QVector<QString> wordList; //仅用来保存单词
    QVector<QVector<QString>> usrHasRead; //用户已背单词
    QVector<QVector<QString>> usrErrorWord; //用户的错词
    QVector<QVector<QString>> usrCollectWord; //用户的收藏
    QVector<QVector<QString>> usrTranRecord; //用户的翻译记录

    //一些列关于单词的变量
    int i; //单词总数
    int i1; //下面这些int是为了完成随机排序
    int i2;
    int i3;
    int i4;
    int j1;
    int j2;
    QString string; //保存单词
    QString string1; //正确翻译
    QString string2; //下面三个都是错误翻译
    QString string3;
    QString string4;

    SoundHttp *sound; //声音 来源

    QString Convert(QString str); //中文转换成url编码，以从网上获取发音文件
};

#endif // USERINTERFACE_H

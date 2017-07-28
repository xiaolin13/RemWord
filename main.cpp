#include "login.h"
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>
#include <QSplashScreen>//显示程序所用到的QT类
#include <QtSql> //将数据库里的单词信息存进vector中
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QCoreApplication>

void createDatabase()
{
    QDir dir; //初试程序，一开始存储数据库的路径不存在则创建路径
    if(!dir.exists(".\\database"))
    {
        dir.mkpath(".\\database");
    }
    if(!dir.exists(".\\picture"))
    {
        dir.mkpath(".\\picture");
    }
    if(!dir.exists(".\\Temp"))
    {
        dir.mkpath(".\\Temp");
    }
    QFile file;
    if(!file.exists(".\\database\\userInfo.db")) //初试程序，一开始存储数据库的文件不存在则创建文件
    {
        QSqlDatabase maindb=QSqlDatabase::addDatabase("QSQLITE"); //设置数据库类型为sqlite
        maindb.setDatabaseName(".\\database\\userInfo.db"); //设置数据库名字
        maindb.open();   //打开数据库
        QSqlQuery query;
        //建表，用户信息表
        query.exec("create table users(userName varchar primary key,userPass varchar,userEmail varchar)");
        query.exec("create table usersrecord(id integer primary key autoincrement,userName varchar,userPass varchar)");
        maindb.close();
    }
    if(!file.exists(".\\database\\danci.db"))
    {
        QSqlDatabase worddb=QSqlDatabase::addDatabase("QSQLITE");
        worddb.setDatabaseName(".\\database\\danci.db");
        worddb.open();
        QSqlQuery query;
        query.exec("create table allwords(id integer primary key autoincrement,word varchar,TRL1 varchar,TRL2 varchar,TRL3 varchar,TRL4 varchar,TRL5 varchar)");
        worddb.close();
    }
    if(!file.exists(".\\database\\usrWordrecord.db"))
    {
        QSqlDatabase wordrecord=QSqlDatabase::addDatabase("QSQLITE");
        wordrecord.setDatabaseName(".\\database\\usrWordrecord.db");
        wordrecord.open();
        QString str="123";
        QSqlQuery query;
        query.exec("create table hasRead(id integer primary key,usr varchar,word varchar,TRL1 varchar,TRL2 varchar,TRL3 varchar,TRL4 varchar,TRL5 varchar)");
        query.exec("create table ErrorWords(id integer primary key,usr varchar,word varchar,TRL1 varchar,TRL2 varchar,TRL3 varchar,TRL4 varchar,TRL5 varchar)");
        query.exec("create table CollectWords(id integer primary key,usr varchar,word varchar,TRL1 varchar,TRL2 varchar,TRL3 varchar,TRL4 varchar,TRL5 varchar)");
        query.exec("create table translaterecord(id integer primary key autoincrement,usr varchar,word varchar,TRL1 varchar,TRL2 varchar,TRL3 varchar,TRL4 varchar,TRL5 varchar)");
        query.bindValue(":d",str);
        query.exec();
        wordrecord.close();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    createDatabase();

    login w;    
    w.show();
    return a.exec();
}

void StartProduct()
{
    QSplashScreen *splash=new QSplashScreen; //设置窗口启动画面
    splash->setPixmap(QPixmap(":/background/启动.jpg"));
    splash->show();
    //createDatabase();
    Qt::Alignment topRight=Qt::AlignRight|Qt::AlignTop;
    splash->showMessage(QObject::tr("Setting up the main window..."),topRight,Qt::white); //设置显示文字
    //a.processEvents();
    //login w;
    splash->showMessage(QObject::tr("Loadint modules..."),topRight,Qt::white); //设置显示文字
    splash->showMessage(QObject::tr("Establishing connection..."),topRight,Qt::white); //设置显示文字
    QDateTime n2=QDateTime::currentDateTime(); //当前时间
    QDateTime now;
    do{
        now=QDateTime::currentDateTime();
    } while(n2.secsTo(now)<=1);  //相差一秒后退出
    //w.show();
    //splash->finish(&w);
    delete splash; //删除掉splash
}

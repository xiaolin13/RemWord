#include "userinterface.h"
#include "ui_userinterface.h"

//按钮格式
const QString button_style="QPushButton{background-color:white;\
                            color: black;   border-radius: 10px;  border: 2px groove gray;\
                            border-style: outset;}"
                            "QPushButton:hover{background-color:rgb(60, 220, 255); color: black;}"
                            "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                            border-style: inset; }";

QNetworkAccessManager *manager; //网络翻译

UserInterface::UserInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInterface)
{
    ui->setupUi(this);

    manager=new QNetworkAccessManager(this); //初始化manager

    sound=new SoundHttp(); //初始化发音 头文件

    userButtonstyle(); //按钮样式 列表背景透明
    dishb(); //隐藏标题栏

    connect(ui->userTILineEdit,SIGNAL(returnPressed()),this,SLOT(on_userTButton_clicked())); //回车即可翻译
    connect(ui->userlistWidget,SIGNAL(currentRowChanged(int)),this,SLOT(changeWidget(int))); //listwidget变化然后stackwidget发生变化
    connect(manager,SIGNAL(finished(QNetworkReply*)),SLOT(finishedSlot(QNetworkReply*))); //网络请求结束后连接函数
    connect(ui->userTILineEdit,SIGNAL(textChanged(QString)),this,SLOT(buttonSlot()));   //翻译按钮的可用形，当空文本时不能翻译
    connect(ui->UsrReadWordListWidget,SIGNAL(currentRowChanged(int)),ui->usrReadListWidget,SLOT(setCurrentIndex(int))); // 背单词界面 listwidget变化然后stackwidget发生变化
    connect(ui->UsrReadWordListWidget,SIGNAL(currentRowChanged(int)),this,SLOT(setWordData())); //响应函数，把数据放上去
    //connect(ui->userTILineEdit,SIGNAL(returnPressed()),this,SLOT(on_userTButton_clicked()));
    blow=new blowup();
    connect(blow,SIGNAL(refresh()),this,SLOT(refreshPicture()));
}

UserInterface::~UserInterface()
{
    delete ui;
}

void UserInterface::on_userExitButton_clicked() //离开时将vector数据插入数据库
{
    QSqlDatabase worddb=QSqlDatabase::addDatabase("QSQLITE"); //初始化数据库
    worddb.setDatabaseName(".\\database\\usrWordrecord.db"); //在离开时把数据放进数据库
    worddb.open();
    QSqlQuery query;
    for(int i=usrHasRead.count()-1;i>=0;--i) //插入已经背完的单词
    {
        query.prepare("insert or ignore into hasRead values(:d7,:d,:d1,:d2,:d3,:d4,:d5,:d6)"); //将数据插入
        query.bindValue(":d7",usrHasRead[i].at(0));
        query.bindValue(":d",usrHasRead[i].at(1));
        query.bindValue(":d1",usrHasRead[i].at(2));
        query.bindValue(":d2",usrHasRead[i].at(3));
        query.bindValue(":d3",usrHasRead[i].at(4));
        query.bindValue(":d4",usrHasRead[i].at(5));
        query.bindValue(":d5",usrHasRead[i].at(6));
        query.bindValue(":d6",usrHasRead[i].at(7));
        query.exec();
    }
    for(int i=usrErrorWord.count()-1;i>=0;--i) //插入背错的单词
    {
        query.prepare("insert or ignore into ErrorWords values(:d7,:d,:d1,:d2,:d3,:d4,:d5,:d6)"); //将数据插入
        query.bindValue(":d7",usrErrorWord[i].at(0));
        query.bindValue(":d",usrErrorWord[i].at(1));
        query.bindValue(":d1",usrErrorWord[i].at(2));
        query.bindValue(":d2",usrErrorWord[i].at(3));
        query.bindValue(":d3",usrErrorWord[i].at(4));
        query.bindValue(":d4",usrErrorWord[i].at(5));
        query.bindValue(":d5",usrErrorWord[i].at(6));
        query.bindValue(":d6",usrErrorWord[i].at(7));
        query.exec();
    }
    for(int i=usrCollectWord.count()-1;i>=0;--i) //插入收集的单词
    {
        query.prepare("insert or ignore into CollectWords values(:d7,:d,:d1,:d2,:d3,:d4,:d5,:d6)"); //将数据插入
        query.bindValue(":d7",usrCollectWord[i].at(0));
        query.bindValue(":d",usrCollectWord[i].at(1));
        query.bindValue(":d1",usrCollectWord[i].at(2));
        query.bindValue(":d2",usrCollectWord[i].at(3));
        query.bindValue(":d3",usrCollectWord[i].at(4));
        query.bindValue(":d4",usrCollectWord[i].at(5));
        query.bindValue(":d5",usrCollectWord[i].at(6));
        query.bindValue(":d6",usrCollectWord[i].at(7));
        query.exec();
    }
    for(int i=0,j=1;i<usrTranRecord.count()&&j<=20;i++,j++) //插入翻译记录到数据库中
    {
        query.prepare("insert into translaterecord values(:d7,:d,:d1,:d2,:d3,:d4,:d5,:d6)"); //将翻译记录插入
        query.bindValue(":d7",j);
        query.bindValue(":d",usrTranRecord[i].at(1));
        query.bindValue(":d1",usrTranRecord[i].at(2));
        query.bindValue(":d2",usrTranRecord[i].at(3));
        query.bindValue(":d3",usrTranRecord[i].at(4));
        query.bindValue(":d4",usrTranRecord[i].at(5));
        query.bindValue(":d5",usrTranRecord[i].at(6));
        query.bindValue(":d6",usrTranRecord[i].at(7));
        query.exec();
    }
    worddb.close();
    usrHasRead.clear();
    usrErrorWord.clear();
    usrCollectWord.clear();
    usrTranRecord.clear();
    close();
    emit createWindow();
}

void UserInterface::on_userCloseButton_clicked() //离开时将vector数据插入数据库
{
    QSqlDatabase worddb=QSqlDatabase::addDatabase("QSQLITE"); //初始化数据库
    worddb.setDatabaseName(".\\database\\usrWordrecord.db"); //在离开时把数据放进数据库
    worddb.open();
    QSqlQuery query;

    for(int i=usrHasRead.count()-1;i>=0;--i) //插入已经背完的单词
    {
        query.prepare("insert or ignore into hasRead values(:d7,:d,:d1,:d2,:d3,:d4,:d5,:d6)"); //将数据插入
        query.bindValue(":d7",usrHasRead[i].at(0).toInt());
        query.bindValue(":d",usrHasRead[i].at(1));
        query.bindValue(":d1",usrHasRead[i].at(2));
        query.bindValue(":d2",usrHasRead[i].at(3));
        query.bindValue(":d3",usrHasRead[i].at(4));
        query.bindValue(":d4",usrHasRead[i].at(5));
        query.bindValue(":d5",usrHasRead[i].at(6));
        query.bindValue(":d6",usrHasRead[i].at(7));
        query.exec();
    }
    for(int i=usrErrorWord.count()-1;i>=0;--i) //插入背错的单词
    {
        query.prepare("insert or ignore into ErrorWords values(:d7,:d,:d1,:d2,:d3,:d4,:d5,:d6)"); //将数据插入
        query.bindValue(":d7",usrErrorWord[i].at(0).toInt());
        query.bindValue(":d",usrErrorWord[i].at(1));
        query.bindValue(":d1",usrErrorWord[i].at(2));
        query.bindValue(":d2",usrErrorWord[i].at(3));
        query.bindValue(":d3",usrErrorWord[i].at(4));
        query.bindValue(":d4",usrErrorWord[i].at(5));
        query.bindValue(":d5",usrErrorWord[i].at(6));
        query.bindValue(":d6",usrErrorWord[i].at(7));
        query.exec();
    }
    for(int i=usrCollectWord.count()-1;i>=0;--i) //插入收集的单词
    {
        query.prepare("insert or ignore into CollectWords values(:d7,:d,:d1,:d2,:d3,:d4,:d5,:d6)"); //将数据插入
        query.bindValue(":d7",usrCollectWord[i].at(0).toInt());
        query.bindValue(":d",usrCollectWord[i].at(1));
        query.bindValue(":d1",usrCollectWord[i].at(2));
        query.bindValue(":d2",usrCollectWord[i].at(3));
        query.bindValue(":d3",usrCollectWord[i].at(4));
        query.bindValue(":d4",usrCollectWord[i].at(5));
        query.bindValue(":d5",usrCollectWord[i].at(6));
        query.bindValue(":d6",usrCollectWord[i].at(7));
        query.exec();
    }
    for(int i=0,j=1;i<usrTranRecord.count()&&j<=20;i++,j++) //将翻译记录插入到数据库中
    {
        query.prepare("delete from translaterecord where id=:d8"); //先删除数据再插入
        query.bindValue(":d8",j);
        query.exec();
        query.prepare("insert into translaterecord values(:d7,:d,:d1,:d2,:d3,:d4,:d5,:d6)"); //将翻译记录插入
        query.bindValue(":d7",j);
        query.bindValue(":d",usrTranRecord[i].at(1));
        query.bindValue(":d1",usrTranRecord[i].at(2));
        query.bindValue(":d2",usrTranRecord[i].at(3));
        query.bindValue(":d3",usrTranRecord[i].at(4));
        query.bindValue(":d4",usrTranRecord[i].at(5));
        query.bindValue(":d5",usrTranRecord[i].at(6));
        query.bindValue(":d6",usrTranRecord[i].at(7));
        query.exec();
    }
    worddb.close();
    close();
}

void UserInterface::on_userCenterButton_clicked()
{
    createCenter();//生成个人中心界面
}

//三种鼠标响应事件，使能拖动无标题栏窗口
void UserInterface::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        uiIsdrag=true;
        uiPointDrag=event->pos();
    }
}

void UserInterface::mouseMoveEvent(QMouseEvent *event)
{
    if(uiIsdrag)
    {
        move(event->pos()-uiPointDrag+pos());
    }
    return QWidget::mouseMoveEvent(event);
}

void UserInterface::mouseReleaseEvent(QMouseEvent *)
{
    uiIsdrag=false;  //做判断
}

//消去标题栏
void UserInterface::dishb()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAutoFillBackground(true);
}

//绘制背景
void UserInterface::drawbackground(QPalette &palette)
{
    setPalette(palette);
    ui->userCenterButton->setPalette(palette);
    ui->userCloseButton->setPalette(palette);
    ui->userlistWidget->setPalette(palette);
    ui->UsrReadWordListWidget->setPalette(palette);
}

//按钮样式 列表背景透明
void UserInterface::userButtonstyle()
{
    ui->userlistWidget->setCurrentRow(1);//默认为第一页
    ui->stackedWidget->setCurrentIndex(1);
    ui->userILabel->hide();

    ui->userTRButton->setCursor(QCursor(Qt::PointingHandCursor)); //翻译记录按钮
    ui->userTRButton->setStyleSheet(button_style);

    ui->userTButton->setCursor(QCursor(Qt::PointingHandCursor)); //翻译按钮
    ui->userTButton->setStyleSheet(button_style);
    ui->userTButton->setEnabled(false);

    ui->voicepushButton->setEnabled(false);

    ui->userExitButton->setCursor(QCursor(Qt::PointingHandCursor)); //退出登陆按钮
    ui->userExitButton->setStyleSheet(button_style);

    ui->userCloseButton->setCursor(QCursor(Qt::PointingHandCursor)); //关闭按钮
    ui->userCloseButton->setFlat(true); //右上角关闭按钮
    ui->userCloseButton->setStyleSheet("QPushButton:hover{border-image:url(:/pic/logo.png);color:red}");

    ui->userCenterButton->setCursor(QCursor(Qt::PointingHandCursor)); //个人中心按钮
    ui->userCenterButton->setFlat(true); //右上角关闭按钮
    ui->userCenterButton->setStyleSheet("QPushButton:hover{border-image:url(:/pic/logo.png);color:red}");

    ui->userlistWidget->setStyleSheet("background-color:transparent"); //控件透明

    ui->pictureButton->setFlat(true); //头像按钮透明

    ui->InterButton->setCursor(QCursor(Qt::PointingHandCursor)); //在线查询按钮
    ui->InterButton->setFlat(true); //右上角关闭按钮
    ui->InterButton->setStyleSheet("QPushButton:hover{border-image:url(:/pic/logo.png);color:red}");
    ui->InterButton->hide();

    ui->LocalButton->setCursor(QCursor(Qt::PointingHandCursor)); //本地查询按钮
    ui->LocalButton->setFlat(true); //右上角关闭按钮
    ui->LocalButton->setStyleSheet("QPushButton:hover{border-image:url(:/pic/logo.png);color:red}");
    ui->LocalButton->hide();

    //背单词界面按钮
    ui->UsrCollectButton->setCursor(QCursor(Qt::PointingHandCursor)); //收藏按钮
    ui->UsrCollectButton->setStyleSheet(button_style);

    ui->WordButton1->setCursor(QCursor(Qt::PointingHandCursor));
    ui->WordButton1->setStyleSheet(button_style);

    ui->WordButton2->setCursor(QCursor(Qt::PointingHandCursor));
    ui->WordButton2->setStyleSheet(button_style);

    ui->WordButton3->setCursor(QCursor(Qt::PointingHandCursor));
    ui->WordButton3->setStyleSheet(button_style);

    ui->WordButton4->setCursor(QCursor(Qt::PointingHandCursor));
    ui->WordButton4->setStyleSheet(button_style);

    ui->UsrReadWordListWidget->setStyleSheet("background-color:transparent");
}

//窗口切换
void UserInterface::changeWidget(int i)
{
    ui->userTextBrower->clear(); //切换界面时把textBrower除掉
    ui->InterButton->hide(); //切换界面时在线翻译按钮隐藏
    ui->LocalButton->hide(); //切换界面时本地翻译按钮隐藏
    ui->userTILineEdit->clear(); //切换界面时清空输入栏
    if(i==0)
    {
        ui->stackedWidget->setCurrentIndex(0);

    }
    else if(i==1)
    {
        ui->userILabel->hide();
        ui->UserLLabel->show();
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if(i==2)
    {
        ui->UserLLabel->hide();
        ui->userILabel->show();
        ui->stackedWidget->setCurrentIndex(1);
    }
}

//翻译向网络发出请求 或是请求查询容器
void UserInterface::on_userTButton_clicked()
{
    if(ui->userlistWidget->currentRow()==2) //在线翻译
    {
        QNetworkRequest *request=new QNetworkRequest();
        request->setUrl(QUrl("http://fanyi.baidu.com/v2transapi")); //网络表头
        QByteArray postData;
        QByteArray ssport=ui->userTILineEdit->text().toUtf8(); //获取要翻译的文本并将其转换成utf-8格式
        if(ui->userComboBox->currentIndex()==0) //英译中
        {
            postData.append("from=en&to=zh&query=").append(ssport);
            reply=manager->post(*request,postData);  //发送请求
        }
        else if(ui->userComboBox->currentIndex()==1) //中译英
        {
            postData.append("from=zh&to=en&query=").append(ssport);
            reply=manager->post(*request,postData);
        }
    } //第一部分，向网络发出信号
    else if(ui->userlistWidget->currentRow()==1) //本地翻译
    {
        int q;
        ui->InterButton->hide(); //隐藏跳转按钮
        if(ui->userComboBox->currentIndex()==0) //英译中
        {
            bool is=true;
            QString str=ui->userTILineEdit->text(); //获取翻译的文本
            for(q=0;q<wordInformation.count();++q) //再本地词库搜索
            {
                if(str==wordInformation[q].at(1))
                {
                    is=false;
                    break;
                }
            }
            if(is)
            {
                 ui->userTextBrower->setText("本地查询失败，点击跳到");
                 ui->InterButton->show();
            }
            else
            {
                ui->userTextBrower->setText(wordInformation[q].at(2)+"\n"+wordInformation[q].at(3)+"\n"+wordInformation[q].at(4)+"\n"+wordInformation[q].at(5)+"\n"+wordInformation[q].at(6));
                bool are=true; //将结果保存到翻译记录
                int q1; //记录
                QString j;
                for(q1=0;q1<usrTranRecord.count();++q1) //判断有没有该记录，有就标记为删除
                {
                    if(ui->userTILineEdit->text()==usrTranRecord[q1].at(2)) //如果有则直接跳出
                    {
                        are=false;
                        break;
                    }
                }
                if(are) //直接插入
                {
                    usrTranRecord.push_back({NULL,userinterName,ui->userTILineEdit->text(),wordInformation[q].at(2),wordInformation[q].at(3),wordInformation[q].at(4),wordInformation[q].at(5),wordInformation[q].at(6)});
                }
                else //先删后增
                {
                    usrTranRecord.remove(q1);
                    usrTranRecord.push_back({j,userinterName,ui->userTILineEdit->text(),wordInformation[q].at(2),wordInformation[q].at(3),wordInformation[q].at(4),wordInformation[q].at(5),wordInformation[q].at(6)});
                }
            }
        }
        else if(ui->userComboBox->currentIndex()==1) //中译英
        {
            bool is=true;
            QString str=ui->userTILineEdit->text();
            for(q=0;q<wordInformation.count();++q)
            {
                if(str==wordInformation[q].at(2)||str==wordInformation[q].at(3)||str==wordInformation[q].at(4)||str==wordInformation[q].at(5)||str==wordInformation[q].at(6))
                {
                    is=false;
                    break;
                }
            }
            if(is)
            {
                ui->userTextBrower->setText("本地查询失败，点击跳到");
                ui->InterButton->show();
            }
            else {
                ui->userTextBrower->setText(wordInformation[q].at(1));
                bool are=true; //将结果保存到翻译记录
                int q1; //记录
                QString j;
                for(q1=0;q1<usrTranRecord.count();++q1) //判断有没有该记录，有就标记为删除
                {
                    if(ui->userTILineEdit->text()==usrTranRecord[q1].at(3)||ui->userTILineEdit->text()==usrTranRecord[q1].at(4)
                            ||ui->userTILineEdit->text()==usrTranRecord[q1].at(5)
                            ||ui->userTILineEdit->text()==usrTranRecord[q1].at(6)
                            ||ui->userTILineEdit->text()==usrTranRecord[q1].at(7)) //如果有则直接跳出
                    {
                        are=false;
                        break;
                    }
                }
                if(are) //直接插入
                {
                    usrTranRecord.push_back({NULL,userinterName,wordInformation[q].at(1),ui->userTILineEdit->text(),NULL,NULL,NULL,NULL});
                }
                else //先删后增
                {
                    usrTranRecord.remove(q1);
                    usrTranRecord.push_back({j,userinterName,wordInformation[q].at(1),ui->userTILineEdit->text(),NULL,NULL,NULL,NULL});
                }
            }
        }
    }
}

void UserInterface::finishedSlot(QNetworkReply *reply)  //网络应答结束，处理返回来的结果
{
        if(reply->error()==QNetworkReply::NoError)
        {
            QByteArray bytes=reply->readAll();
            QString string=QString::fromLocal8Bit(bytes);
            int i=90;  //有效数据从90位开始
            QString string1;
            while(string.at(i)!='"')  //获取有效编码
            {
                 string1+=string.at(i);
                i++;
            }
            if(ui->userComboBox->currentIndex()==0) //中译英
            {
                 do{
                    int idx=string1.indexOf("\\u");
                    QString strHex=string1.mid(idx,6);
                    strHex=strHex.replace("\\u",QString());
                    int nHex=strHex.toInt(0,16);
                    string1.replace(idx,6,QChar(nHex));
                    } while(string1.indexOf("\\u")!=-1); //将网络返回的json格式编码转变成汉字
                ui->userTextBrower->setText(string1);
                //将翻译记录放进容器里
                QString j; //保留存储的id
                bool is=true; //判断要不要先删后减
                int q;
                for(q=0;q<usrTranRecord.count();++q)  //查询有无重复，有重复则先删后增
                {
                    if(string1==usrTranRecord[q].at(7)||string1==usrTranRecord[q].at(3)||string1==usrTranRecord[q].at(4)||string1==usrTranRecord[q].at(5)||string1==usrTranRecord[q].at(6))
                    {
                        is=false;
                        break;
                    }
                }
                if(is)  //直接将翻译结果插入容器
                {
                    usrTranRecord.push_back({NULL,userinterName,ui->userTILineEdit->text(),string1,NULL,NULL,NULL,NULL});
                }
                else //先删后增，先保留id序号
                {
                    usrTranRecord.remove(q);
                    usrTranRecord.push_back({j,userinterName,ui->userTILineEdit->text(),string1,NULL,NULL,NULL,NULL});
                }
            }
         else if(ui->userComboBox->currentIndex()==1)  //英译中
            {
                ui->userTextBrower->setText(string1);
                bool is=true; //插入到翻译记录的容器中
                QString j;
                int q;
                for(q=0;q<usrTranRecord.count();++q) //查询有无重复，有重复则先删后增
                {
                    if(string1==usrTranRecord[q].at(2))
                    {
                        is=false;
                        break;
                    }
                }
                if(is) //直接将翻译结果插入容器
                {
                    usrTranRecord.push_back({NULL,userinterName,string1,ui->userTILineEdit->text(),NULL,NULL,NULL,NULL});
                }
                else //先删后增，先保留id序号
                {
                    usrTranRecord.remove(q);
                    usrTranRecord.push_back({j,userinterName,string1,ui->userTILineEdit->text(),NULL,NULL,NULL,NULL});
                }
            }
        }
        else  //出现网络故障
        {
            qDebug()<<"handle errors here";
            QVariant statusCodeV=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            qDebug("found error ... codec: %d %d\n",statusCodeV.toInt(),(int)reply->error());
            ui->userTextBrower->append("网络连接失败，请返回本地查询");
            ui->LocalButton->show();
        }
        reply->deleteLater();
}
void UserInterface::buttonSlot() //lineEdit为空时不能翻译
{
    if(ui->userTILineEdit->text().length()>=1)
    {
        ui->userTButton->setEnabled(true);
        ui->voicepushButton->setEnabled(true);
        QString str=ui->userTILineEdit->text();
        //还存在一个缺陷，一办中文一半英文时尚未解决
        //还有个麻烦是输入数字字符串会当成英文输入
        bool is=str.toLocal8Bit().length()==str.length(); //判断输入是汉字还是英文,然后自动切换。
        if(!is)
        {
            ui->userComboBox->setCurrentIndex(1);
        }
        else
        {
            ui->userComboBox->setCurrentIndex(0);
        }
        ui->userComboBox->setEnabled(false);   //程序根据所输入的字符自行判断，用户不能选择
    }
    else
    {
        ui->userTButton->setEnabled(false);
        ui->voicepushButton->setEnabled(false);
        ui->userComboBox->setEnabled(true); //用户尚未输入字符串时可以选择
    }
}

void UserInterface::createCenter()  //生成个人中心界面
{
    useC=new UserCenter();
    useC->UserCenterName=userinterName;
    useC->createCenterName();
    useC->show();
}

void UserInterface::createName() //生成用户名字
{
    ui->userNameLabel->setText(userinterName);
}

void UserInterface::setPicture() //生成头像
{
    QPixmap picture;
    QString str=".\\picture\\"+userinterName+".jpg";
    QFile file(str);
    if(file.exists())
    {
        picture.load(str);
        ui->pictureButton->setIcon(picture);
    }
    else {
        picture.load(":/background/空.jpg");
        ui->pictureButton->setIcon(picture);
    }
}

void UserInterface::on_pictureButton_clicked()  //放大头像
{
    blow->picturePath=".\\picture\\"+userinterName+".jpg";
    blow->createPicture();
    blow->show();
}

void UserInterface::refreshPicture() //刷新界面
{
    setPicture();
}

void UserInterface::setData() //生成用户单词数据
{
    QSqlDatabase usrdb=QSqlDatabase::addDatabase("QSQLITE");
    usrdb.setDatabaseName(".\\database\\usrWordrecord.db");
    usrdb.open();
    QSqlQuery query;
    query.prepare("select * from hasRead where usr==:d"); //将已背单词放进容器中
    query.bindValue(":d",userinterName);
    query.exec();
    while(query.next())
    {
        usrHasRead.push_back({query.value(0).toString(),query.value(1).toString(),
                              query.value(2).toString(),query.value(3).toString(),
                              query.value(4).toString(),query.value(5).toString(),
                              query.value(6).toString(),query.value(7).toString()});
    }
    query.prepare("select * from ErrorWords where usr==:d"); //将错误单词放进容器中
    query.bindValue(":d",userinterName);
    query.exec();
    while(query.next())
    {
        usrErrorWord.push_back({query.value(0).toString(),query.value(1).toString(),
                              query.value(2).toString(),query.value(3).toString(),
                              query.value(4).toString(),query.value(5).toString(),
                              query.value(6).toString(),query.value(7).toString()});
    }
    query.prepare("select * from CollectWords where usr==:d"); //将收藏单词放进容器中
    query.bindValue(":d",userinterName);
    query.exec();
    while(query.next())
    {
        usrCollectWord.push_back({query.value(0).toString(),query.value(1).toString(),
                              query.value(2).toString(),query.value(3).toString(),
                              query.value(4).toString(),query.value(5).toString(),
                              query.value(6).toString(),query.value(7).toString()});
    }
    query.prepare("select * from translaterecord where usr==:d"); //将收藏单词放进容器中
    query.bindValue(":d",userinterName);
    query.exec();
    while(query.next())
    {
        usrTranRecord.push_back({query.value(0).toString(),query.value(1).toString(),
                              query.value(2).toString(),query.value(3).toString(),
                              query.value(4).toString(),query.value(5).toString(),
                              query.value(6).toString(),query.value(7).toString()});
    }
    usrdb.close();


    QSqlDatabase worddb=QSqlDatabase::addDatabase("QSQLITE"); //数据库初试操作
    worddb.setDatabaseName(".\\database\\danci.db");
    worddb.open();
    QSqlQuery query1;
    query1.exec("select * from allwords");
    while(query1.next()) //将数据插入容器 所有单词
    {
        wordInformation.push_back({query1.value(0).toString(),query1.value(1).toString(),
                                  query1.value(2).toString(),query1.value(3).toString(),
                                  query1.value(4).toString(),query1.value(5).toString(),
                                  query1.value(6).toString()});
        wordList.push_back(query1.value(1).toString());
    }
    worddb.close();
}

void UserInterface::setWordData() //放置单词数据到界面上
{
    if(ui->UsrReadWordListWidget->currentRow()==1) //所有单词界面
    {
        ui->AllWordstextBrowser->clear(); //先清空
        for(int i=0;i<wordInformation.count();++i)
        {
            ui->AllWordstextBrowser->append(wordInformation[i].at(1)+" "+wordInformation[i].at(2)+" "+wordInformation[i].at(3)+" "+wordInformation[i].at(4)+" "+wordInformation[i].at(5)+" "+wordInformation[i].at(6));
        }
    }
    if(ui->UsrReadWordListWidget->currentRow()==2)
    {
        ui->HasReadWordstextBrowser->clear(); //先清空
        for(int i=0;i<usrHasRead.count();++i)
        {
            ui->HasReadWordstextBrowser->append(usrHasRead[i].at(2)+" "+usrHasRead[i].at(3)+" "+usrHasRead[i].at(4)+" "+usrHasRead[i].at(5)+" "+usrHasRead[i].at(6)+" "+usrHasRead[i].at(7));
        }
    }
    if(ui->UsrReadWordListWidget->currentRow()==3)
    {
        ui->ErrorWordtextBrowser->clear(); //先清空
        for(int i=0;i<usrErrorWord.count();++i)
        {
            ui->ErrorWordtextBrowser->append(usrErrorWord[i].at(2)+" "+usrErrorWord[i].at(3)+" "+usrErrorWord[i].at(4)+" "+usrErrorWord[i].at(5)+" "+usrErrorWord[i].at(6)+" "+usrErrorWord[i].at(7));
        }
    }
    if(ui->UsrReadWordListWidget->currentRow()==4)
    {
        ui->CollectWordtextBrowser->clear();  //先清空
        for(int i=0;i<usrCollectWord.count();++i)
        {
            ui->CollectWordtextBrowser->append(usrCollectWord[i].at(2)+" "+usrCollectWord[i].at(3)+" "+usrCollectWord[i].at(4)+" "+usrCollectWord[i].at(5)+" "+usrCollectWord[i].at(6)+" "+usrCollectWord[i].at(7));
        }
    }
    return;
}

void UserInterface::Shengchengdanci() //生成按钮上的翻译
{
    i=wordInformation.count(); //单词总数
    j2=usrHasRead.count();
    if(i==j2)
    {
        QMessageBox::information(this,tr("good"),tr("你已经背完单词"));
        return;
    }
    bool is=true;
    while(1) //不会在已背单词中找
    {
        i1=qrand()%i;  //产生随机数
        string=wordInformation[i1].at(1); //获得对应的单词及翻译string
        string1=wordInformation[i1].at(2)+" "+wordInformation[i1].at(3)+"\n"+wordInformation[i1].at(4)+" "+wordInformation[i1].at(5)+" "+wordInformation[i1].at(6);
        for(int q=0;q<usrHasRead.count();++q) //循环查找
        {
            int q1=usrHasRead[q].indexOf(string);
            if(q1!=-1)
            {
                is=false;
                break;
            }
        }
        if(is)
        {
            break;
        }
    }
    i2=qrand()%i;
    if(i1==i2)
    {
        i2=qrand()%i;
    }
    i3=qrand()%i;
    if(i1==i3||i3==i2)
    {
        i3=qrand()%i;
    }
    i4=qrand()%i;
    if(i4==i1||i4==i2||i4==i3)
    {
        i4=qrand()%i;
    }
    string2=wordInformation[i2].at(2)+" "+wordInformation[i2].at(3)+"\n"+wordInformation[i2].at(4)+" "+wordInformation[i2].at(5)+" "+wordInformation[i2].at(6);
    string3=wordInformation[i3].at(2)+" "+wordInformation[i3].at(3)+"\n"+wordInformation[i3].at(4)+" "+wordInformation[i3].at(5)+" "+wordInformation[i3].at(6);
    string4=wordInformation[i4].at(2)+" "+wordInformation[i4].at(3)+"\n"+wordInformation[i4].at(4)+" "+wordInformation[i4].at(5)+" "+wordInformation[i4].at(6);
    ui->UsrWordLabel->setText(string);
    j1=qrand()%4;
    if(j1==0)   //设置随机事件，将翻译汉语放在上面
    {
        ui->WordButton1->setText(string1);
        ui->WordButton2->setText(string2);
        ui->WordButton3->setText(string3);
        ui->WordButton4->setText(string4);
    }
    else if(j1==1)
    {
        ui->WordButton1->setText(string2);
        ui->WordButton2->setText(string1);
        ui->WordButton3->setText(string3);
        ui->WordButton4->setText(string4);
    }
    else if(j1==2)
    {
        ui->WordButton1->setText(string3);
        ui->WordButton2->setText(string2);
        ui->WordButton3->setText(string1);
        ui->WordButton4->setText(string4);
    }
    else if(j1==3)
    {
        ui->WordButton1->setText(string4);
        ui->WordButton2->setText(string2);
        ui->WordButton3->setText(string3);
        ui->WordButton4->setText(string1);
    }
}

void UserInterface::on_WordButton1_clicked() //第一个按钮
{
    int q=0;
    if(ui->WordButton1->text()==string1) //如果对则刷新，不对则返回重选
    {
        usrHasRead.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                             ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                             wordInformation[i1].at(4),wordInformation[i1].at(5),
                             wordInformation[i1].at(6)});//如果对则插入单词到已背单词中
        Shengchengdanci();
    }
    else {
        bool is=true; //用作判断是否已经存在错词中，如果存在则删除再插，不存在则直接插入
        ui->UsrWordWarningLabel->setText("<font color=red size=4><b>错误</b></font>");
        for(q=0;q<usrErrorWord.count();++q)
        {
            int i=usrErrorWord[q].indexOf(string);
            if(i!=-1)
            {
                is=false;
                break;
            }
        }
        if(is) //不存在则直接插入
        {
            usrErrorWord.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                                    ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                                    wordInformation[i1].at(4),wordInformation[i1].at(5),
                                    wordInformation[i1].at(6)});
        }
        else //存在的画先删除 再插入
        {
            usrErrorWord.remove(q);
            usrErrorWord.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                                    ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                                    wordInformation[i1].at(4),wordInformation[i1].at(5),
                                    wordInformation[i1].at(6)});
        }
        return;
    }
}

void UserInterface::on_WordButton2_clicked() //第二个按钮
{
    int q=0;
    if(ui->WordButton2->text()==string1) //如果对则刷新，不对则返回重选
    {
        usrHasRead.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                             ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                             wordInformation[i1].at(4),wordInformation[i1].at(5),
                             wordInformation[i1].at(6)});//如果对则插入单词到已背单词中
        Shengchengdanci();
    }
    else {
        bool is=true; //用作判断是否已经存在错词中，如果存在则删除再插，不存在则直接插入
        ui->UsrWordWarningLabel->setText("<font color=red size=4><b>错误</b></font>");
        for(q=0;q<usrErrorWord.count();++q)
        {
            int i=usrErrorWord[q].indexOf(string);
            if(i!=-1)
            {
                is=false;
                break;
            }
        }
        if(is) //不存在则直接插入
        {
            usrErrorWord.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                                    ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                                    wordInformation[i1].at(4),wordInformation[i1].at(5),
                                    wordInformation[i1].at(6)});
        }
        else //存在的画先删除 再插入
        {
            usrErrorWord.remove(q);
            usrErrorWord.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                                    ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                                    wordInformation[i1].at(4),wordInformation[i1].at(5),
                                    wordInformation[i1].at(6)});
        }
        return;
    }
}

void UserInterface::on_WordButton3_clicked() //第三个按钮
{
    int q=0;
    if(ui->WordButton3->text()==string1) //如果对则刷新，不对则返回重选
    {
        usrHasRead.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                             ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                             wordInformation[i1].at(4),wordInformation[i1].at(5),
                             wordInformation[i1].at(6)});//如果对则插入单词到已背单词中
        Shengchengdanci();
    }
    else {
        bool is=true; //用作判断是否已经存在错词中，如果存在则删除再插，不存在则直接插入
        ui->UsrWordWarningLabel->setText("<font color=red size=4><b>错误</b></font>");
        for(q=0;q<usrErrorWord.count();++q)
        {
            int i=usrErrorWord[q].indexOf(string);
            if(i!=-1)
            {
                is=false;
                break;
            }
        }
        if(is) //不存在则直接插入
        {
            usrErrorWord.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                                    ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                                    wordInformation[i1].at(4),wordInformation[i1].at(5),
                                    wordInformation[i1].at(6)});
        }
        else //存在的画先删除 再插入
        {
            usrErrorWord.remove(q);
            usrErrorWord.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                                    ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                                    wordInformation[i1].at(4),wordInformation[i1].at(5),
                                    wordInformation[i1].at(6)});
        }
        return;
    }
}

void UserInterface::on_WordButton4_clicked() //第四个按钮
{
    int q=0;
    if(ui->WordButton4->text()==string1) //如果对则刷新，不对则返回重选
    {
        usrHasRead.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                             ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                             wordInformation[i1].at(4),wordInformation[i1].at(5),
                             wordInformation[i1].at(6)});//如果对则插入单词到已背单词中
        Shengchengdanci();
    }
    else {
        bool is=true; //用作判断是否已经存在错词中，如果存在则删除再插，不存在则直接插入
        ui->UsrWordWarningLabel->setText("<font color=red size=4><b>错误</b></font>");
        for(q=0;q<usrErrorWord.count();++q)
        {
            int i=usrErrorWord[q].indexOf(string);
            if(i!=-1)
            {
                is=false;
                break;
            }
        }
        if(is) //不存在则直接插入
        {
            usrErrorWord.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                                    ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                                    wordInformation[i1].at(4),wordInformation[i1].at(5),
                                    wordInformation[i1].at(6)});
        }
        else //存在的画先删除 再插入
        {
            usrErrorWord.remove(q);
            usrErrorWord.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                                    ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                                    wordInformation[i1].at(4),wordInformation[i1].at(5),
                                    wordInformation[i1].at(6)});
        }
        return;
    }
}

void UserInterface::on_UsrCollectButton_clicked()
{
    int q;
    bool is=true;
    for(q=0;q<usrCollectWord.count();++q)
    {
        int i=usrCollectWord[q].indexOf(string);
        if(i!=-1)
        {
            is=false;
            break;
        }
    }
    if(is) //不存在则直接插入
    {
        usrCollectWord.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                                ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                                wordInformation[i1].at(4),wordInformation[i1].at(5),
                                wordInformation[i1].at(6)});
    }
    else //存在的画先删除 再插入
    {
        usrCollectWord.remove(q);
        usrCollectWord.push_back({wordInformation[i1].at(0),userinterName,wordInformation[i1].at(1)
                                ,wordInformation[i1].at(2),wordInformation[i1].at(3),
                                wordInformation[i1].at(4),wordInformation[i1].at(5),
                                wordInformation[i1].at(6)});
    }
    ui->UsrWordWarningLabel->setText("<font color=red size=4><b>收藏成功</b></font>");
}


void UserInterface::on_userTRButton_clicked() //点击翻译记录按钮
{
    ui->userTextBrower->clear(); //先清空textBrower
    ui->InterButton->hide(); //隐藏跳转按钮
    ui->LocalButton->hide(); //隐藏跳转按钮
    ui->userTILineEdit->clear(); //将文本清空
    for(int q=usrTranRecord.count()-1;q>=0;--q) //从下往上读取
    {
        ui->userTextBrower->append(usrTranRecord[q].at(2)+" "+usrTranRecord[q].at(3)+" "+usrTranRecord[q].at(4)+" "+usrTranRecord[q].at(5)+" "+usrTranRecord[q].at(6)+" "+usrTranRecord[q].at(7));
    }
}

void UserInterface::on_InterButton_clicked() //切换到在线翻译
{
    QString str=ui->userTILineEdit->text(); //先保存文本
    ui->userlistWidget->setCurrentRow(2);
    ui->userTILineEdit->setText(str);
    on_userTButton_clicked(); //等于按下翻译按钮
}

void UserInterface::on_LocalButton_clicked() //切换到本地翻译
{
    QString str=ui->userTILineEdit->text(); //先保存文本
    ui->userlistWidget->setCurrentRow(1);
    ui->userTILineEdit->setText(str);
    on_userTButton_clicked(); //等于按下翻译按钮
}

void UserInterface::on_voicepushButton_clicked() //翻译时的发音
{
    QString str=ui->userTILineEdit->text();//或得文本
    int i=1;
    if(ui->userComboBox->currentIndex()==1) //如果是获取中文发音的话得先转码
    {
        str=Convert(str);
        i=2;   //然后设置i值，以获取中文发音路径
    }
    if(ui->userlistWidget->currentRow()==1)
    {
        on_LocalButton_clicked();
    }
    else if(ui->userlistWidget->currentRow()==2)
    {
        on_InterButton_clicked();
    }    
    sound->Click(str,i);
}

void UserInterface::on_qmediaButton1_clicked()  //背单词界面得发音
{
    QString str=string;  //单词文本
    sound->Click(str,1);
}

QString UserInterface::Convert(QString str)
{
    QString strInput = str;
    if(str.isEmpty())
    {
        return "";
    }

    QByteArray ba = strInput.toUtf8();
    QByteArray byteArrayPercentEncoded = ba.toPercentEncoding();
    return QString(byteArrayPercentEncoded);
}

#include "soundhttp.h"

const QString path="Temp\\";
const QString Fpath="https://sp0.baidu.com/-rM1hT4a2gU2pMbgoY3K/gettts?lan=uk&text=";
const QString Lpath="&spd=2&source=alading";

const QString Fpath2="http://fanyi.baidu.com/gettts?lan=zh&text=";
const QString Lpath2="&spd=5&source=web";

SoundHttp::SoundHttp() : QWidget()
{
    manager=new QNetworkAccessManager();
    player=new QMediaPlayer();
}

void SoundHttp::Click(QString text,int i) //接口函数 ，用来调用发音
{
    QString string=text;
    if(i==1)   //中文发音和英文发音路径不同，需要区分
        url=Fpath+text+Lpath;
    else if(i==2)
    {
        url=Fpath2+text+Lpath2;
    }

    fileName=path+string+".mp3";  //直接以url编码存储文件，读取时也以url格式读取
    file=new QFile(fileName);
    if(file->exists())
    {
        Read();
        return;
    }

    if(!file->open(QIODevice::WriteOnly))
    {
        qDebug()<<"file open error";
        delete file;
        file=0;
        return;
    }
    startRequest(url);
}

void SoundHttp::startRequest(QUrl url)
{
    reply=manager->get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(httpFinished()));
    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
}

void SoundHttp::httpReadyRead()
{
    if(file) file->write(reply->readAll());
}

void SoundHttp::Read()  //发音
{
    player->setMedia(QUrl::fromLocalFile(fileName));
    player->setVolume(50);
    player->play();
}

void SoundHttp::httpFinished() //请求结束后
{
    Read();
    file->flush();
    file->close();
    reply->deleteLater();
    reply=0;
    delete file;
    file=0;
}

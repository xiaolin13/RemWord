#ifndef SOUNDHTTP_H
#define SOUNDHTTP_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QMediaPlayer>
#include <QWidget>

class SoundHttp : QWidget
{
    Q_OBJECT

public:
    SoundHttp();
    void startRequest(QUrl url);
    void Click(QString text, int i);

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QUrl url;
    QFile *file;
    QMediaPlayer *player;
    QString fileName;
    void Read();

private slots:
    void httpFinished();
    void httpReadyRead();
};

#endif // SOUNDHTTP_H

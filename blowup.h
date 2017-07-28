#ifndef BLOWUP_H
#define BLOWUP_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>

#include <QMouseEvent> //响应鼠标事件

namespace Ui {
class blowup;
}

class blowup : public QDialog
{
    Q_OBJECT

public:
    explicit blowup(QWidget *parent = 0);
    ~blowup();

    virtual void mousePressEvent(QMouseEvent *); //响应鼠标点击事件
    virtual void mouseMoveEvent(QMouseEvent *);   //响应鼠标移动事件
    virtual void mouseReleaseEvent(QMouseEvent *);  //响应鼠标释放事件

    QString picturePath;
    void createPicture(); //生成头像

signals:
    void refresh();

private slots:
    void on_pushButton_clicked();

    void on_blowClose_clicked();

    void on_blowupButton_clicked();

private:
    Ui::blowup *ui;

    bool isDrag; //使鼠标能够拖拽窗体
    QPoint bDragPoint;

    void head(); //消去标题栏
};

#endif // BLOWUP_H

#include "blowup.h"
#include "ui_blowup.h"

blowup::blowup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::blowup)
{
    ui->setupUi(this);
    head(); //消去标题栏
}

blowup::~blowup()
{
    delete ui;
}

void blowup::createPicture() //生成头像
{
    QPixmap pixmap;
    QFile file(picturePath);
    if(file.exists())
    {
        pixmap.load(picturePath);
        ui->blowupButton->setIcon(pixmap);
    }
    else
    {
        pixmap.load(":/background/空.jpg");
        ui->blowupButton->setIcon(pixmap);
    }
}

//三种鼠标响应事件，使能拖动无标题栏窗口
void blowup::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        isDrag=true;
        bDragPoint=event->pos();
    }
}

void blowup::mouseMoveEvent(QMouseEvent *event)
{
    if(isDrag)
    {
        move(event->pos()-bDragPoint+pos());
    }
    return QWidget::mouseMoveEvent(event);
}

void blowup::mouseReleaseEvent(QMouseEvent *)
{
    isDrag=false;  //做判断
}

void blowup::head() //窗体样式
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    ui->blowClose->setCursor(QCursor(Qt::PointingHandCursor));
    ui->blowClose->setFlat(true); //右上角关闭按钮
    ui->blowupButton->setFlat(true); //放图片的按钮透明
    ui->blowClose->setStyleSheet("QPushButton:hover{border-image:url(:/pic/logo.png);color:red}");
}

void blowup::on_blowClose_clicked()
{
    close();
    emit refresh();
}

void blowup::on_blowupButton_clicked()
{
    QString fangdafiles = QFileDialog::getOpenFileName(this, //只能打开指定文件格式jpg
                tr("打开图片"),
                QDir::currentPath(),
                "Images (*.jpg)",
                0);
   if(fangdafiles.isEmpty()) //判断文件存不存在
   {
        return;
    }
    QFile file(picturePath); //如果文件存在则先删除再存储新图片
    if(file.exists())
    {
        file.remove(); //删除文件
    }
    QImage fangdaimage;
    fangdaimage.load(fangdafiles);
    ui->blowupButton->setIcon(QPixmap::fromImage(fangdaimage));
    fangdaimage.save(picturePath);  //保存文件
    update();
}

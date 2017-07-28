#ifndef CODEAREA_H
#define CODEAREA_H

#include <QWidget>
#include <QList>
#include <QRect>
#include <QPainterPath>
#include <QTime>

#define DEF_CODECOUNT 4

#define DEF_NOISYPOINTCOUNT 200
#define DEF_CONVERSEROTATE 10
#define DEF_CONVERSESCALE 15

class CodeArea : public QWidget
{
    Q_OBJECT

public:
    explicit CodeArea(QWidget *parent = 0);
    ~CodeArea();
    QString m_sCode="";

    void replaceCodePic(); //替换当前验证码，并且文本也换掉

    void setCodeCount(int nCodeCount); //设置验证码位数

    void setNoisyPointCount(int nNoisyPointCount); //设置噪点数，即干扰点

    bool checkCode(QString sCode); //检查验证码对不对，程序里没有用到

    void updateCode(); //更新验证码

protected:
    void paintEvent(QPaintEvent *event);

private:
    inline void initCodeRange(); //初始化验证码
    inline void initCodeColorList(); //初始化验证码颜色
    inline void updateLoginCode(); //更新验证码文本
    inline void updateCodePic(); //更新验证码的图片
    inline void drawOutline(QPainter &painter); //画边框
    inline void drawCode(QPainter &painter,int nCodeIndex); //绘制验证码
    inline void drawNoisyPoint(QPainter &painter); //绘制噪点
    inline void drawConversion(QPainter &painter); //绘制倾斜
    inline void setCodePix(const QList<QPainterPath *> &lCodePic); //设置验证码图片

private:
    QStringList m_slCodeRange; //验证码取值范围
    QStringList m_slLoginCode; //当前验证码string
    QPainterPath *m_pCodePic; //每个验证码图片绘制路径
    QList<QPainterPath *> m_lCodePic; //整幅验证码文本途径
    QList<Qt::GlobalColor> m_lCodeColor; //验证码颜色

    //设定各个默认值
    int m_nNoisyPointCount = DEF_NOISYPOINTCOUNT; //噪点
    int m_nConverseRotate = DEF_CONVERSEROTATE; //旋转
    int m_nConverseScale = DEF_CONVERSESCALE; //放大
    int m_nCodeCount = DEF_CODECOUNT; //数量

signals:

public slots:
};

#endif // CODEAREA_H

#include "drawwidget.h"
#include <QtGui>
#include <QPen>

DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);    //对窗体背景色的设置
    setPalette(QPalette(Qt::white));
    pix =new QPixmap(size());      	//此QPixmap对象用来准备随时接收绘制的内容
    pix->fill(Qt::white);           //填充背景色为白色
    setMinimumSize(600,400);      	//设置绘制区窗体的最小尺寸
    //初始化图形相关信息

}

void DrawWidget::setStyle(int s)
{
    style = s;
}

void DrawWidget::setWidth(int w)
{
    weight = w;
}

void DrawWidget::setColor(QColor c)
{
    color = c;
}

void DrawWidget::mousePressEvent(QMouseEvent *e)
{
    startPos = e->pos();
}

void DrawWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPainter *painter = new QPainter;

    QPen pen;
    pen.setStyle((Qt::PenStyle)style);
    pen.setWidth(weight);
    pen.setColor(color);

    painter->begin(pix);
    painter->setPen(pen);
    painter->drawLine(startPos, e->pos());
    painter->end();
    startPos =e->pos();
    update();
}

void DrawWidget::paintEvent(QPaintEvent *)
{
    SetRect();
    QPainter painter(this);
    QRect target(m_rectList[0][0], m_rectList[0][1], m_rectList[0][2], m_rectList[0][3]);
    painter.drawPixmap(target, *pix);
//    QRect rectWnd = this->rect();
//    // 绘制文字
//    QPainter painter(this);
//    // 设置画笔颜色
//    painter.setPen(QPen(Qt::black, 4, Qt::DashDotLine, Qt::RoundCap));

//    painter.drawRect(300, 300, 700, 400);

//    painter.drawEllipse(QPoint(500, 500), 50, 50);
//    //分离intersection交点
//    painter.setBrush(QBrush(QColor(0, 255, 0)));
////    if(!m_polygon.empty()) {
////        painter.drawPolygon(&m_polygon[0], m_polygon.size());
////    }
}

void DrawWidget::resizeEvent(QResizeEvent *event)
{
    if(height()>pix->height()||width()>pix->width())
    {
        QPixmap *newPix = new QPixmap(size());
        newPix->fill(Qt::white);
        QPainter p(newPix);
        p.drawPixmap(QPoint(0,0),*pix);
        pix = newPix;
    }
    QWidget::resizeEvent(event);
}

void DrawWidget::SetRect()
{
    m_rectList.reserve(10);
    m_rectList[0].reserve(4);
    m_rectList[0][0] = 300;
    m_rectList[0][1] = 300;
    m_rectList[0][2] = 700;
    m_rectList[0][3] = 400;
}

void DrawWidget::SetEllipse()
{
    m_ellipseList.reserve(10);
    m_ellipseList[0] = {500, 500, 50, 50};
}

void DrawWidget::SetPolygon()
{
    m_polygonList.reserve(10);
    m_polygonList[0] = {};
}

void DrawWidget::clear()
{
    QPixmap *clearPix =new QPixmap(size());
    clearPix->fill(Qt::white);
    pix = clearPix;
    update();
}

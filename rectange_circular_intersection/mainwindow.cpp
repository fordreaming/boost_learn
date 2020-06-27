#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <boost/geometry.hpp>
#include <boost/geometry/io/io.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/spirit/include/karma.hpp>
#include <iostream>
#include <QPainter>
#include <sstream>
#include <QStringList>
#include <QMessageBox>

const static QString POLYGON_HEAD = "Polygon((";
const static QString POLYGON_END = "))";

using std::cout;
using std::ios;
using std::string;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_paint = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GetVertical()
{
    if(m_string.empty()) {
        return;
    }
    QString qStr = QString::fromStdString(m_string);
    //split
    //remove "POLYGON((
    int leftRemove = POLYGON_HEAD.size();
    qStr.remove(0, leftRemove);

    int lastIndex = qStr.lastIndexOf(POLYGON_END);
    int rightRemove = POLYGON_END.size();
    qStr.remove(lastIndex, rightRemove + 1);
    QStringList strList = qStr.split(",");
    //
    if(strList.empty()) {
        return ;
    }
    for(int i = 0; i < strList.size(); ++i) {
        QStringList tempList = strList[i].split(" ");
        double pointX = tempList[0].toDouble();
        double pointY = tempList[1].toDouble();
        QPoint point(pointX, pointY);
        m_polygon.append(point);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if(!m_paint) {
        return ;
    }
    //重绘事件
    Q_UNUSED(event)

    QRect rectWnd = this->rect();
    // 绘制文字
    QPainter painter(this);
    // 设置画笔颜色
    painter.setPen(QPen(Qt::black, 4, Qt::DashDotLine, Qt::RoundCap));

    painter.drawRect(300, 300, 200, 400);

    painter.drawEllipse(QPoint(500, 500), 50, 50);
    //分离intersection交点
//    painter.setBrush(Qt::NoBrush);
    painter.setBrush(QBrush(QColor(0, 255, 0)));
    painter.drawPolygon(&m_polygon[0], m_polygon.size());
}

void MainWindow::on_btnCalc_clicked()
{
    typedef boost::geometry::model::d2::point_xy<double> point;
    typedef boost::geometry::model::polygon<point> polygon;

    const double buffer_distance = 50; // radius of circle
    const int points_per_circle = 72;
    boost::geometry::strategy::buffer::distance_symmetric<double> distance_strategy(buffer_distance);
    boost::geometry::strategy::buffer::join_round join_strategy(points_per_circle);
    boost::geometry::strategy::buffer::end_round end_strategy(points_per_circle);
    boost::geometry::strategy::buffer::point_circle circle_strategy(points_per_circle);
    boost::geometry::strategy::buffer::side_straight side_strategy;

    boost::geometry::model::multi_polygon<polygon> result;

    point pt;
    boost::geometry::read_wkt("POINT(500 500)", pt); // center of circle

    boost::geometry::buffer(pt, result,
                distance_strategy, side_strategy,
                join_strategy, end_strategy, circle_strategy);
    // first item of result is circle with 1 radius and (5,5) point as center
    // result should have 1 polygon
    polygon rect; // your rectangle
    boost::geometry::read_wkt("POLYGON((300 300,300 700,500 700,500 300,300 300))",rect);

    std::deque<polygon> intersectionGeometry;
    boost::geometry::intersection(rect,result.front(),intersectionGeometry);
    if (intersectionGeometry.size() == 1)
        m_intersectionStream << boost::geometry::wkt(intersectionGeometry.front()) << std::endl;

    m_string = m_intersectionStream.str();
}

void MainWindow::on_btnDraw_clicked()
{
    if(m_string.empty()) {
        QMessageBox::information(this, tr("warning!"), tr("please press calculate first"));
        return;
    }
    m_paint = true;
    GetVertical();
    repaint();
}

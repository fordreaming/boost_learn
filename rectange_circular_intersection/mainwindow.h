#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <drawwidget.h>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

using std::fstream;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void GetVertical();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_btnCalc_clicked();

    void on_btnDraw_clicked();

    void on_shapeComboBox_activated(int index);

private:
    Ui::MainWindow                  *ui;
    std::ostringstream              m_intersectionStream;
    bool                            m_paint;                    //false: 不重绘        true:重绘
    std::string                     m_string;
    QList<QPoint>                   m_polygon;

    QLabel                          *shapeLabel;
    QWidget                         *m_mainWidget;




};

#endif // MAINWINDOW_H

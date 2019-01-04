#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QSlider>
#include <QDebug>
#include <QPushButton>
#include <QVector>
#include <QPoint>
#include <QPainter>
#include <QLabel>
#include <QStack>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel* label_mode;

    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;

    unsigned int width = 400;
    unsigned int height = 300;

    unsigned int i = 0;

    bool fill = false;

    QPoint p;
    QImage* paper = new QImage (int(width), int(height), QImage::Format_RGB32);

private slots:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

    void setRed(int);
    void setGreen(int);
    void setBlue(int);
    void setFill();
    void setText();

    void Fill();
};

#endif // MAINWINDOW_H

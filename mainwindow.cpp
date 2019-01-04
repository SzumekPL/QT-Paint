#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);

    int width = 400;
    int height = 300;

    this->statusBar()->setSizeGripEnabled(false);
    this->setMinimumSize(width,height);
    this->setMaximumSize(width,height);

    paper->fill(QColor(255,255,255));

    QWidget* window = new QWidget();

    QSlider* slider_red = new QSlider( Qt::Horizontal );
    QSlider* slider_green = new QSlider( Qt::Horizontal );
    QSlider* slider_blue = new QSlider( Qt::Horizontal );

    QSpinBox* spinBox_red = new QSpinBox();
    QSpinBox* spinBox_green = new QSpinBox();
    QSpinBox* spinBox_blue = new QSpinBox();

    label_mode = new QLabel( (fill)?"Zapelnianie":"Rysowanie" );

    QPushButton* mode = new QPushButton("MODE");

    slider_red->setValue(0);
    slider_blue->setValue(0);
    slider_green->setValue(0);
    slider_red->setRange(0 , 255);
    slider_green->setRange(0 , 255);
    slider_blue->setRange(0 , 255);

    spinBox_red->setValue(slider_red->value());
    spinBox_green->setValue(slider_green->value());
    spinBox_blue->setValue(slider_blue->value());
    spinBox_red->setRange(0 , 255);
    spinBox_green->setRange(0 , 255);
    spinBox_blue->setRange(0 , 255);

    QObject::connect(slider_red, SIGNAL(valueChanged(int)),
                     this, SLOT(setRed(int)));
    QObject::connect(slider_red, SIGNAL(valueChanged(int)),
                     spinBox_red, SLOT(setValue(int)));

    QObject::connect(slider_green, SIGNAL(valueChanged(int)),
                     this, SLOT(setGreen(int)));
    QObject::connect(slider_green, SIGNAL(valueChanged(int)),
                     spinBox_green, SLOT(setValue(int)));

    QObject::connect(slider_blue, SIGNAL(valueChanged(int)),
                     this, SLOT(setBlue(int)));
    QObject::connect(slider_blue, SIGNAL(valueChanged(int)),
                     spinBox_blue, SLOT(setValue(int)));

    QObject::connect(mode, SIGNAL(clicked()),
                     this, SLOT(setFill()));
    QObject::connect(mode, SIGNAL(clicked()),
                     this, SLOT(setText()));

    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *buf_red_layout = new QHBoxLayout;
    QHBoxLayout *buf_green_layout = new QHBoxLayout;
    QHBoxLayout *buf_blue_layout = new QHBoxLayout;
    QHBoxLayout *buf_mode_layout = new QHBoxLayout;

    buf_red_layout->addWidget(slider_red);
    buf_red_layout->addWidget(spinBox_red);

    main_layout->addLayout(buf_red_layout);

    buf_green_layout->addWidget(slider_green);
    buf_green_layout->addWidget(spinBox_green);

    main_layout->addLayout(buf_green_layout);

    buf_blue_layout->addWidget(slider_blue);
    buf_blue_layout->addWidget(spinBox_blue);

    main_layout->addLayout(buf_blue_layout);

    buf_mode_layout->addWidget(mode);
    buf_mode_layout->addWidget(label_mode);

    main_layout->addLayout(buf_mode_layout);

    window->setLayout(main_layout);

    window->show();
    delete buf_red_layout;
    delete buf_green_layout;
    delete buf_blue_layout;
    delete buf_mode_layout;
}

void MainWindow::setText()
{
    label_mode->setText((fill)?"Zapelnianie":"Rysowanie");
    return;
}

void MainWindow::setFill()
{
    fill = !fill;
    return;
}

void MainWindow::setRed(int value)
{
    red = uchar(value);
    return;
}

void MainWindow::setGreen(int value)
{
    green = uchar(value);
    return;
}

void MainWindow::setBlue(int value)
{
    blue = uchar(value);
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    p = event->pos();
    if(!fill)
    {
        update();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    p = event->pos();
    if(fill)
    {
        Fill();
    }
}

void MainWindow::Fill()
{
    QStack <QPoint> Q;
    Q.push(p);
    QColor change = paper->pixelColor(p);
    while (!Q.isEmpty())
    {
        QPoint buf = Q.pop();
        if(buf.x() < 0 || buf.y() < 0 || buf.x() > int(width) || buf.y() > int(height))
        {
            continue;
        }
        if(paper->pixelColor(buf) == change)
        {
            int w = buf.x();
            int e = buf.x();

            while(w >= 0 && paper->pixelColor(w,buf.y()) == change )
            {
                w--;
            }

            while(e < width && paper->pixelColor(e,buf.y()) == change )
            {
                e++;
            }

            for (int i = w+1; i < e; ++i)
            {
                uchar* ptr = paper->bits();
                ptr[(unsigned(buf.y())*width + unsigned(i)) * 4 + 0] = blue;
                ptr[(unsigned(buf.y())*width + unsigned(i)) * 4 + 1] = green;
                ptr[(unsigned(buf.y())*width + unsigned(i)) * 4 + 2] = red;
            }

            for (int i = w+1; i < e; ++i)
            {
                if (buf.y()+1 < height && paper->pixelColor(i,buf.y()+1) == change)
                {
                    Q.push(QPoint(i,buf.y()+1));
                }
                if (buf.y()-1 >= 0 && paper->pixelColor(i,buf.y()-1) == change)
                {
                    Q.push(QPoint(i,buf.y()-1));
                }
            }
        }
    }
    update();
    return;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter pixel(this);

    uchar *ptr = paper->bits();

    if(p.y() >= 0 && p.x() >= 0 && !fill)
    {
        ptr[(unsigned(p.y())*width + unsigned(p.x())) * 4 + 0] = blue;
        ptr[(unsigned(p.y())*width + unsigned(p.x())) * 4 + 1] = green;
        ptr[(unsigned(p.y())*width + unsigned(p.x())) * 4 + 2] = red;
    }

    pixel.drawImage(0,0,*paper);
}



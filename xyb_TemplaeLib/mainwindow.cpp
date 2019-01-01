#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QLineSeries>
#include <QAreaSeries>
#include <QConicalGradient>
#include <QLinearGradient>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    groupComboBox *yy = new groupComboBox(this);
//    yy->setTitle(QString("ahhaha"));
//    yy->show();

//    dataBase mydata;
//    mydata.init("QMYSQL","qt_data","localhost","root","ADMIN");
//    QSqlQuery test;
//    mydata.query(&test,"SELECT * FROM train");
//    int fieldNo = test.record().indexOf("Station");
//    while (test.next()) {
//        QString country = test.value(fieldNo).toString();
//        qDebug()<<country;
//    }
//    qDebug()<<test.size();

    QWidget *m = new QWidget(this);
    m->setStyleSheet("border-image: url(:/img/bg.jpg);");

    QWidget *w = new QWidget(this);
    w->setStyleSheet("border-image: url();");

    QWidget *w1 = new QWidget();
    QWidget *w2 = new QWidget();
    QWidget *w3 = new QWidget();
    QWidget *w4 = new QWidget();

    QString qss = "background-color:rgb(150,162,176,120);border-radius:10px;"
                  "border: 2px solid rgb(61, 252, 255)";

    w1->setStyleSheet(qss);
    w2->setStyleSheet(qss);
    w3->setStyleSheet(qss);
    w4->setStyleSheet(qss);

    QGridLayout *thisLayout = new QGridLayout();
    thisLayout->addWidget(w1,0,0);
    thisLayout->addWidget(w2,0,1);
    thisLayout->addWidget(w3,1,0);
    thisLayout->addWidget(w4,1,1);
    w->setLayout(thisLayout);

    QWidget *h = new QWidget(this);
    h->setStyleSheet("background-color:rgb(255,255,255,0);border-image: url();");

    h1 = new groupPolarChart(this);
    h1->returnChart()->setMargins(QMargins(0,0,0,0));
    h1->returnChart()->legend()->hide();
    h1->setChartBackgroundColor(QColor(255,225,255,0));

    h1->setChartPoltAreaBackgroundColor(true,QBrush(QColor(50,50,50,230)));//QColor(50,50,50,230)
    h1->setAxisLabelsColor(groupPolarChart::allAxisf,QColor(0, 252, 0));
    h1->setGridLineColor(groupPolarChart::angularAxisF,QColor(80,80,80));
    h1->setGridLineColor(groupPolarChart::radialAxisF,QColor(80,80,80));
    h1->returnValueAxis(groupPolarChart::angularAxisF)->setLinePen(QPen(QBrush(QColor(61, 252, 255)),2));

    QLineSeries *series3 = new QLineSeries();
    series3->setColor(QColor(255,255,255,0));
    series3->append(0, 0);
    series3->append(70, 1);


    QLineSeries *series4 = new QLineSeries();
    series4->append(0, 0);
    series4->append(90, 1);
    series4->append(80, 1);

    QAreaSeries *series5 = new QAreaSeries();
    series5->setBorderColor(QColor(255,255,255,0));
    series5->setUpperSeries(series3);
    series5->setLowerSeries(series4);
    QLinearGradient mycolor(QPointF(1,3),QPointF(0,0));
    mycolor.setColorAt(0,QColor(160,160,160));
    mycolor.setColorAt(1,QColor(50,50,50,100));
    mycolor.setCoordinateMode(QGradient::ObjectBoundingMode);
    series5->setBrush(mycolor);
    series5->setOpacity(0.5);

    h1->addSeries(series3);
    h1->addSeries(series4);
    h1->addSeries(series5);
    h1->setAxisRange(groupPolarChart::angularAxisF,0,360);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(h1);
    h->setLayout(hLayout);

    QStackedLayout *stklay = new QStackedLayout();
    stklay->setStackingMode(QStackedLayout::StackAll);
    stklay->addWidget(w);
    stklay->addWidget(h);
    m->setLayout(stklay);

    this->setCentralWidget(m);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updataPos()));
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}
/**
*   @project         xyb_TemplaeLib
*   @brief           定时器超时更新位置
*   @param a         参数1：
*   @param b         参数2：
*   @author          XuYuanbing
*   @data            2018-12-29
*/
void MainWindow::updataPos()
{

    QLineSeries *mylin1 = (QLineSeries*)h1->returnChart()->series().at(0);
    mylin1->replace(mylin1->pointsVector().at(1),QPointF(mylin1->pointsVector().at(1) + QPointF(1,1)));
    //qDebug()<<mylin1->pointsVector();

    QLineSeries *mylin2 = (QLineSeries*)h1->returnChart()->series().at(1);
    mylin2->replace(mylin2->pointsVector().at(1),QPointF(mylin2->pointsVector().at(1) + QPointF(1,1)));
    mylin2->replace(mylin2->pointsVector().at(2),QPointF(mylin2->pointsVector().at(2) + QPointF(1,1)));
    //qDebug()<<mylin2->pointsVector();
    timer->start(50);

    return;
}

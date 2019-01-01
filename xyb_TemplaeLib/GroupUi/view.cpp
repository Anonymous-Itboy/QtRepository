/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "view.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
//#include "callout.h"
#include <QtGui/QMouseEvent>
#include <QDebug>
#include <QHBoxLayout>

View::View(QWidget *parent,
           int xMin,
           int xMax,
           int yMin,
           int yMax)
    : QGraphicsView(new QGraphicsScene, parent),
      m_coordX(0),
      m_coordY(0),
      m_chart(0)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    xMinValue = xMin;
    xMaxValue = xMax;
    yMinValue = yMin;
    yMaxValue = yMax;

    // chart
    m_chart = new QChart;
    m_chart->legend()->hide();
    m_chart->setBackgroundBrush(QBrush(QColor(25,25,25)));
    m_chart->setMargins(QMargins(0,0,0,0));


//    QLineSeries *series = new QLineSeries;
//    series->append(1, 3);
//    series->append(4, 5);
//    series->append(5, 4.5);
//    series->append(7, 1);
//    series->append(11, 2);
//    m_chart->addSeries(series);

//    QSplineSeries *series2 = new QSplineSeries;
//    series2->append(1.6, 1.4);
//    series2->append(2.4, 3.5);
//    series2->append(3.7, 2.5);
//    series2->append(7, 4);
//    series2->append(10, 2);
//    m_chart->addSeries(series2);

    QPen myPen;
    myPen.setStyle(Qt::DotLine);
    series20 = new QLineSeries;
    series21 = new QLineSeries;
    series20->setPen(myPen);
    series21->setPen(myPen);
    series20->setColor(QColor(0,255,0));
    series21->setColor(QColor(0,255,0));
    m_chart->addSeries(series20);
    m_chart->addSeries(series21);

    m_chart->createDefaultAxes();
    m_chart->setAcceptHoverEvents(true);
    m_chart->axisX()->setGridLineVisible(false);
    m_chart->axisY()->setGridLineVisible(false);
    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setBrush(QColor(0,255,0));
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setBrush(QColor(0,255,0));

    m_chart->axisX()->setRange(xMinValue,xMaxValue);
    m_chart->axisY()->setRange(yMinValue,yMaxValue);
    m_chart->axisY()->setLabelsColor(QColor(0,255,0));
    m_chart->installEventFilter(this);
    this->setMouseTracking(true);
    this->setContentsMargins(QMargins(0,0,0,0));
    this->setStyleSheet("border:none;padding:0px;margin:0px;background:rgb(0,0,0);");
}

void View::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         m_chart->resize(event->size());
         m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height() - 20);
         m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height() - 20);
    }
    QGraphicsView::resizeEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    if(m_chart->mapToValue(event->pos()).x() >= 0 && m_chart->mapToValue(event->pos()).y() >= 0)
    {
        m_coordX->show();
        m_coordY->show();
        m_coordX->setPos(event->pos().x()+5,event->pos().y()-40);
        m_coordY->setPos(event->pos().x()+5,event->pos().y()-20);
        m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).x()));
        m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));
        series20->clear();
        series20->append(0,m_chart->mapToValue(event->pos()).y());
        series20->append(xMaxValue, m_chart->mapToValue(event->pos()).y());
        series21->clear();
        series21->append(m_chart->mapToValue(event->pos()).x(),0);
        series21->append(m_chart->mapToValue(event->pos()).x(), yMaxValue);
        emit mouseEvent(m_chart->mapToValue(event->pos()).x(),m_chart->mapToValue(event->pos()).y());
        series20->show();
        series21->show();
    }
    else {
        m_coordX->hide();
        m_coordY->hide();
        series20->hide();
    }

//    qDebug()<<"Y:"<<m_chart->mapToValue(event->pos()).y();

    QGraphicsView::mouseMoveEvent(event);
}
bool View::eventFilter(QObject *watched, QEvent *event)
{

    return QWidget::eventFilter(watched,event);
}

QChart* View::returnChart()
{
    return m_chart;
}

void View::tooltip(QPointF point, bool state)
{
//    if (m_tooltip == 0)
//        m_tooltip = new Callout(m_chart);

//    if (state) {
//        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
//        m_tooltip->setAnchor(point);
//        m_tooltip->setZValue(11);
//        m_tooltip->updateGeometry();
//        m_tooltip->show();
//    } else {
//        m_tooltip->hide();
//    }
}
void View::updataMouseEvent(qreal x,qreal y)
{
    series21->clear();
    series21->show();
    series21->append(x,0);
    series21->append(x, yMaxValue);
    return;
}
void View::updataMouseEvent2(qreal x,qreal y)
{
    series20->show();
    series21->show();
    series20->clear();
    series20->append(0,y);
    series20->append(xMaxValue,y);
    series21->clear();
    series21->append(x,0);
    series21->append(x, yMaxValue);
    return;
}
void View::clearUi()
{
    m_coordX->hide();
    m_coordY->hide();
    series20->hide();
    series21->hide();
}
void View::leaveEvent(QEvent *)
{
    clearUi();
    emit cleanAll();
}

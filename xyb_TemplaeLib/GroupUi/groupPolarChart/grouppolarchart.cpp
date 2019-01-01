/**
*   @project         FrameDemo
*   @brief           自定义极坐标
*   @author          XuYuanbing
*   @data            2018-08-06
*/
#include <QChartView>
#include <QChart>
#include <QPolarChart>
#include <QHBoxLayout>
#include <QDebug>

#include "grouppolarchart.h"

groupPolarChart::groupPolarChart(QWidget *parent,
                                 const qreal angMin,
                                 const qreal angMax,
                                 const qreal radMin,
                                 const qreal radMax)
    :QWidget(parent)
{
    angularMin = angMin;
    angularMax = angMax;
    radialMin  = radMin;
    radialMax  = radMax;

    pChart = new QPolarChart();
    angularAxis = new QValueAxis();
    angularAxis->setRange(angularMin, angularMax);
    pChart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);

    radialAxis  = new QValueAxis();
    radialAxis->setRange(radialMin, radialMax);
    pChart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    pChart->setMargins(QMargins(0,0,0,0));

    QChartView *chartView = new QChartView();
    chartView->setChart(pChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QHBoxLayout *thisLayout = new QHBoxLayout();
    thisLayout->addWidget(chartView);
    this->setLayout(thisLayout);

    return;
}
/**
*   @project         FrameDemo
*   @brief           设置坐标图例字体颜色
*   @author          XuYuanbing
*   @data            2018-08-06
*/
void groupPolarChart::setChartLegendColor(const QColor color)
{
    pChart->legend()->setLabelColor(color);
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置坐标标题颜色
*   @author          XuYuanbing
*   @data            2018-08-06
*/
void groupPolarChart::setChartTitleColor(const QColor color)
{
    pChart->setTitleBrush(QBrush(color));
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置坐标标题
*   @author          XuYuanbing
*   @data            2018-08-06
*/
void groupPolarChart::setChartTitle(const QString title)
{
    pChart->setTitle(title);
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置坐标系的背景颜色
*   @author          XuYuanbing
*   @data            2018-08-06
*/
void groupPolarChart::setChartBackgroundColor(const QColor color)
{
    pChart->setBackgroundBrush(QBrush(color));
    return;
}
/**
*   @project         FrameDemo
*   @brief           添加数据线
*   @author          XuYuanbing
*   @data            2018-08-06
*/
void groupPolarChart::addSeries(QAbstractSeries *series)
{
    pChart->addSeries(series);
    series->attachAxis(angularAxis);
    series->attachAxis(radialAxis);
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置坐标轴的刻度个数
*   @author          XuYuanbing
*   @data            2018-08-06
*/
void groupPolarChart::setAxisTickCount(groupPolarChartAxisF axis,int num)
{
    switch (axis) {
    case angularAxisF:
    {
        angularAxis->setTickCount(num);
        break;
    }
    case radialAxisF:
    {
        radialAxis->setTickCount(num);
        break;
    }
    default:
        qDebug()<<"axis flag err.axis:"<<axis;
        break;
    }
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置坐标轴的刻度精度
*   @author          XuYuanbing
*   @data            2018-08-06
*/
void groupPolarChart::setAxisLabelFormat(groupPolarChartAxisF axis,const QString &format)
{
    switch (axis) {
    case angularAxisF:
    {
        angularAxis->setLabelFormat(format);
        break;
    }
    case radialAxisF:
    {
        radialAxis->setLabelFormat(format);
        break;
    }
    case allAxisf:
    {
        angularAxis->setLabelFormat(format);
        radialAxis->setLabelFormat(format);
        break;
    }
    default:
        qDebug()<<"axis flag err.axis:"<<axis;
        break;
    }
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置坐标轴的范围
*   @author          XuYuanbing
*   @data            2018-08-06
*/
void groupPolarChart::setAxisRange(groupPolarChartAxisF axis,const qreal min,const qreal max)
{
    switch (axis) {
    case angularAxisF:
    {
        angularAxis->setRange(min, max);
        break;
    }
    case radialAxisF:
    {
        radialAxis->setRange(min, max);
        break;
    }
    default:
        qDebug()<<"axis flag err.axis:"<<axis;
        break;
    }
    return;
}
/**
*   @project         FrameDemo
*   @brief           返回坐标系
*   @author          XuYuanbing
*   @data            2018-08-06
*/
QPolarChart* groupPolarChart::returnChart()
{
    return pChart;
}
/**
*   @project         FrameDemo
*   @brief           返回指定坐标轴
*   @author          XuYuanbing
*   @data            2018-08-06
*/
QValueAxis* groupPolarChart::returnValueAxis(groupPolarChartAxisF axis)
{
    switch (axis) {
    case angularAxisF:
    {
        return angularAxis;
    }
    case radialAxisF:
    {
        return radialAxis;
    }
    default:
        qDebug()<<"axis flag err.axis:"<<axis;
        return nullptr;
    }
}
/**
*   @project         xyb_TemplaeLib
*   @brief           设置圆形区域的颜色 及是否生效
*   @param a         参数1：
*   @param b         参数2：
*   @author          XuYuanbing
*   @data            2018-12-28
*/
void groupPolarChart::setChartPoltAreaBackgroundColor(bool show, const QBrush brush)
{
    pChart->setPlotAreaBackgroundVisible(show);
    if(show)
    {
        pChart->setPlotAreaBackgroundBrush(brush);
        pChart->setMaximumSize(this->parentWidget()->width()/5*4,this->parentWidget()->height()/5*4);

    }
    return;
}
/**
*   @project         xyb_TemplaeLib
*   @brief           设置指定刻度轴 刻度数值颜色
*   @param a         参数1：
*   @param b         参数2：
*   @author          XuYuanbing
*   @data            2018-12-29
*/
void groupPolarChart::setAxisLabelsColor(groupPolarChartAxisF axis, const QColor color)
{
    switch (axis) {
    case angularAxisF:
    {
        angularAxis->setLabelsColor(color);
        break;
    }
    case radialAxisF:
    {
        radialAxis->setLabelsColor(color);
        break;
    }
    case allAxisf:
    {
        angularAxis->setLabelsColor(color);
        radialAxis->setLabelsColor(color);
        break;
    }
    default:
        qDebug()<<"axis flag err.axis:"<<axis;
        break;
    }
    return;
}
/**
*   @project         xyb_TemplaeLib
*   @brief           设置指定轴的网格显示及颜色
*   @param a         参数1：
*   @param b         参数2：
*   @author          XuYuanbing
*   @data            2018-12-29
*/
void groupPolarChart::setGridLineColor(groupPolarChartAxisF axis, const QColor color)
{
    switch (axis) {
    case angularAxisF:
    {
        angularAxis->setGridLineVisible(true);
        angularAxis->setGridLineColor(color);
        break;
    }
    case radialAxisF:
    {
        radialAxis->setGridLineVisible(true);
        radialAxis->setGridLineColor(color);
        break;
    }
    default:
        qDebug()<<"axis flag err.axis:"<<axis;
        break;
    }
    return;
}
/**
*   @project         xyb_TemplaeLib
*   @brief           设置指定轴的网格是否显示
*   @param a         参数1：
*   @param b         参数2：
*   @author          XuYuanbing
*   @data            2018-12-29
*/
void groupPolarChart::setGridLineVisible(groupPolarChartAxisF axis, bool show)
{
    switch (axis) {
    case angularAxisF:
    {
        angularAxis->setGridLineVisible(show);
        break;
    }
    case radialAxisF:
    {
        radialAxis->setGridLineVisible(show);
        break;
    }
    default:
        qDebug()<<"axis flag err.axis:"<<axis;
        break;
    }
    return;
}

/**
*   @project         xyb_TemplaeLib
*   @brief           设置指定轴的轴线颜色
*   @param a         参数1：
*   @param b         参数2：
*   @author          XuYuanbing
*   @data            2018-12-29
*/
void groupPolarChart::setAxisLineColor(groupPolarChartAxisF axis, const QColor color)
{
    switch (axis) {
    case angularAxisF:
    {
        angularAxis->setLinePenColor(color);
        break;
    }
    case radialAxisF:
    {
        radialAxis->setLinePenColor(color);
        break;
    }
    default:
        qDebug()<<"axis flag err.axis:"<<axis;
        break;
    }
    return;
}

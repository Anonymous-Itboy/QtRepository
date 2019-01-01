#ifndef GROUPPOLARCHART_H
#define GROUPPOLARCHART_H

#include <QWidget>
#include <QChart>
#include <QPolarChart>
#include <QValueAxis>

using namespace QtCharts;
class groupPolarChart : public QWidget
{
public:
    groupPolarChart(QWidget *parent = 0,
                    const qreal angularMin = 0,
                    const qreal angularMax =360,
                    const qreal radialMin  =0,
                    const qreal radialMax  =1);
    enum groupPolarChartAxisF {
        angularAxisF = 0,
        radialAxisF  = 1,
        allAxisf     = 2
    };

    void setChartLegendColor(const QColor);
    void setChartTitleColor(const QColor);
    void setChartTitle(const QString);
    void setChartBackgroundColor(const QColor);
    void setChartPoltAreaBackgroundColor(bool,const QBrush);
    void addSeries(QAbstractSeries *series);

    void setAxisTickCount(groupPolarChartAxisF,int);
    void setAxisLabelFormat(groupPolarChartAxisF,const QString &format);
    void setAxisRange(groupPolarChartAxisF,const qreal min,const qreal max);
    void setAxisLabelsColor(groupPolarChartAxisF,const QColor);
    void setGridLineVisible(groupPolarChartAxisF,bool);
    void setGridLineColor(groupPolarChartAxisF,const QColor);
    void setAxisLineColor(groupPolarChartAxisF,const QColor);

    QPolarChart* returnChart();
    QValueAxis* returnValueAxis(groupPolarChartAxisF);


private:
    QPolarChart *pChart;
    QValueAxis *angularAxis;
    QValueAxis *radialAxis;

    qreal angularMin;
    qreal angularMax;
    qreal radialMin;
    qreal radialMax;
};

#endif // GROUPPOLARCHART_H

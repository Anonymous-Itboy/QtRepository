#ifndef QWT_XY_CHART_H
#define QWT_XY_CHART_H

#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_plot.h>
/**
*   @name            %{Current:Name}
*   @brief           摘要
*   @author          XuYuanbing
*   @data            2018-07-31
*/
class Qwt_XY_Chart : public QwtPlot
{
    Q_OBJECT
public:
    enum ColorMap
    {
        RGBMap,
        IndexMap,
        HueMap,
        AlphaMap
    };

    explicit Qwt_XY_Chart(QWidget *parent = nullptr,
                          double xdefMinValue = -1,
                          double xdefMaxValue =  1,
                          double ydefMinValue = -1,
                          double ydefMaxValue =  1,
                          double zdefMinValue = -1,
                          double zdefMaxValue =  1);
    void setAxisTitles(QwtPlot::Axis,const QString);
    void setDataValue(QVector<double>,int,double);
    void setAxisStep(QwtPlot::Axis,double step);
    void reverseAxis(QwtPlot::Axis);
    void setColorMapType(int);
    void setAlpha( int );
    void clearData();

signals:

public slots:

private:
    QwtPlotSpectrogram  *pQwtPlotSpectrogram;
    QwtMatrixRasterData *pQwtMatrixRasterData;
    QwtScaleWidget *pxTopAxis;
    QwtScaleWidget *pxBottomAxis;
    QwtScaleWidget *pyLeftAxis;
    QwtScaleWidget *pyRightAxis;
    QFont axisFont;
};

#endif // QWT_XY_CHART_H

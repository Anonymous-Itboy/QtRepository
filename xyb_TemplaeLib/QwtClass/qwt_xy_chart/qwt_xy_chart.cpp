/**
*   @projectName     FrameDemo
*   @brief           摘要
*   @author          XuYuanbing
*   @data            2018-07-31
*/
#include <QWidget>
#include <QDebug>
#include <QHBoxLayout>

#include <qnumeric.h>
#include <qwt_color_map.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_renderer.h>
#include <qwt_picker_machine.h>

#include "QwtClass/qwt_color_map/qwt_colormap.h"
#include "qwt_xy_chart.h"

class MyZoomer: public QwtPlotZoomer
{
public:
    MyZoomer( QWidget *canvas ):
        QwtPlotZoomer( canvas )
    {
        setStateMachine( new QwtPickerDragPointMachine() );
        setRubberBand( QwtPicker::NoRubberBand );
        setTrackerMode( AlwaysOn );
    }

    virtual QwtText trackerTextF( const QPointF &pos ) const
    {
        QColor bg( Qt::white );
        bg.setAlpha( 200 );
        QwtText text = QwtPlotZoomer::trackerTextF( pos );

        text.setBackgroundBrush( QBrush( bg ) );
        return text;
    }
};

Qwt_XY_Chart::Qwt_XY_Chart(QWidget *parent,
                           double xdefMinValue,
                           double xdefMaxValue,
                           double ydefMinValue,
                           double ydefMaxValue,
                           double zdefMinValue,
                           double zdefMaxValue
                           ) : QwtPlot(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    this->setLayout(new QHBoxLayout( this ));
    pQwtPlotSpectrogram = new QwtPlotSpectrogram();
    pQwtPlotSpectrogram->setRenderThreadCount( 0 );
    pQwtPlotSpectrogram->setCachePolicy( QwtPlotRasterItem::PaintCache );

    QList<double> contourLevels;
    for ( double level = 0.0; level < 10.0; level += 1.0 )
    {
        contourLevels += level;
    }
    pQwtPlotSpectrogram->setContourLevels( contourLevels );

    pQwtMatrixRasterData = new QwtMatrixRasterData();
    pQwtMatrixRasterData->setInterval( Qt::XAxis, QwtInterval( xdefMinValue, xdefMaxValue) );
    pQwtMatrixRasterData->setInterval( Qt::YAxis, QwtInterval( ydefMinValue, ydefMaxValue) );
    pQwtMatrixRasterData->setInterval( Qt::ZAxis, QwtInterval( zdefMinValue, zdefMaxValue) );
    pQwtPlotSpectrogram->setData(pQwtMatrixRasterData);

    pxTopAxis    = axisWidget( QwtPlot::xTop );
    pxBottomAxis = axisWidget( QwtPlot::xBottom );
    pyLeftAxis   = axisWidget( QwtPlot::yLeft );
    pyRightAxis  = axisWidget( QwtPlot::yRight );
    axisFont     = pxBottomAxis->font();

    setAxisScale(QwtPlot::yLeft,  ydefMinValue,ydefMaxValue);
    setAxisScale(QwtPlot::yRight, zdefMinValue,zdefMaxValue);
    setAxisScale(QwtPlot::xBottom,xdefMinValue,xdefMaxValue);

    setColorMapType(0);
    pQwtPlotSpectrogram->setAlpha( 255 );
    replot();
    pQwtPlotSpectrogram->attach( this );

    enableAxis( QwtPlot::yRight);
    pxBottomAxis->setColorBarEnabled( true );
    pyLeftAxis->setColorBarEnabled( true );
    pyRightAxis->setColorBarEnabled( true );
    plotLayout()->setAlignCanvasToScales( true );
    pQwtPlotSpectrogram->setDisplayMode( QwtPlotSpectrogram::ContourMode, true );

    QwtPlotZoomer* zoomer = new MyZoomer( canvas() );
//    zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
//                             Qt::RightButton, Qt::ControlModifier );
//    zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
//                             Qt::RightButton );
//    const QFontMetrics fm( axisWidget( QwtPlot::yLeft )->font() );
//    QwtScaleDraw *sd = axisScaleDraw( QwtPlot::yLeft );
//    sd->setMinimumExtent( fm.width( "100.00" ) );

    const QColor c( Qt::red );
    zoomer->setRubberBandPen( c );
    zoomer->setTrackerPen( c );
    zoomer->activated(false);
//    zoomer->setEnabled(false);

    return;
}
/**
*   @projectName     FrameDemo
*   @brief           设置坐标轴标题
*   @author          XuYuanbing
*   @data            2018-07-31
*/
void Qwt_XY_Chart::setAxisTitles(QwtPlot::Axis axis,const QString title)
{
    QwtText axisTitle(title);
    axisTitle.setFont( axisFont );
    switch (axis) {
    case QwtPlot::xTop:
    {
        pxTopAxis->setTitle( axisTitle );
        break;
    }
    case QwtPlot::xBottom:
    {
        pxBottomAxis->setTitle( axisTitle );
        break;
    }
    case QwtPlot::yLeft:
    {
        pyLeftAxis->setTitle( axisTitle );
        break;
    }
    case QwtPlot::yRight:
    {
        pyRightAxis->setTitle( axisTitle );
        break;
    }
    default:
        qDebug()<<"set qwt XYchart axis title err.check axis flag";
        break;
    }
    return;
}
/**
*   @projectName     FrameDemo
*   @brief           设置数据
*   @author          XuYuanbing
*   @data            2018-07-30
*/
void Qwt_XY_Chart::setDataValue(QVector<double> dataValue,int iColumn,double dFreqFlg)
{
    pQwtMatrixRasterData->setValueMatrix(dataValue,iColumn);
    setAxisTitles(QwtPlot::yRight,QString::number(dFreqFlg).append(QString::fromLocal8Bit("Hz")));
    pQwtPlotSpectrogram->setData(pQwtMatrixRasterData);
    replot();
    return;
}
/**
*   @projectName     FrameDemo
*   @brief           设置指定轴的最小单元刻度
*   axis:坐标轴id  step：单元刻度
*   @author          XuYuanbing
*   @data            2018-07-31
*/
void Qwt_XY_Chart::setAxisStep(QwtPlot::Axis axis, double step)
{
    QwtInterval inter = axisInterval(axis);
    double minVal = inter.minValue();
    double maxVal = inter.maxValue();
    setAxisScale(axis,minVal,maxVal,step);
    return;
}
/**
*   @projectName     FrameDemo
*   @brief           设置指定轴反向
*   @author          XuYuanbing
*   @data            2018-07-31
*/
void Qwt_XY_Chart::reverseAxis(QwtPlot::Axis axis)
{
    QwtInterval inter = axisInterval(axis);
    double minVal = inter.minValue();
    double maxVal = inter.maxValue();
    setAxisScale(axis,maxVal,minVal);
    return;
}
/**
*   @projectName     FrameDemo
*   @brief           设置透明度
*   @author          XuYuanbing
*   @data            2018-07-31
*/
void Qwt_XY_Chart::setAlpha( int alpha )
{
    pQwtPlotSpectrogram->setAlpha( alpha );
    replot();
    return;
}
/**
*   @projectName     FrameDemo
*   @brief           设置色标类型
*   @author          XuYuanbing
*   @data            2018-07-31
*/
void Qwt_XY_Chart::setColorMapType(int type)
{
    pQwtPlotSpectrogram->setColorMap( new qwt_colormap(type) );
    pyRightAxis->setColorMap(pQwtPlotSpectrogram->data()->interval( Qt::ZAxis ),
                             new qwt_colormap(type) );
    replot();
    return;
}
/**
*   @projectName     FrameDemo
*   @brief           清空数据
*   @author          XuYuanbing
*   @data            2018-07-31
*/
void Qwt_XY_Chart::clearData()
{
    pQwtPlotSpectrogram->setData(NULL);
    replot();
    return;
}

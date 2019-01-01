/**
*   @project         FrameDemo
*   @brief           qwt极坐标系
*   @author          XuYuanbing
*   @data            2018-08-07
*/
#include <qwt_polar_picker.h>
#include <qwt_polar_magnifier.h>
#include <qwt_polar_panner.h>
#include "qwt_polar_chart.h"
#include <qwt_picker_machine.h>

#include <qwt_scale_widget.h>
#include <qwt_scale_engine.h>
#include <qlayout.h>
#include "QwtClass/qwt_color_map/qwt_colormap.h"

/**
*   @project         FrameDemo
*   @brief           坐标信息显示
*   @author          XuYuanbing
*   @data            2018-08-07
*/
class MyPicker: public QwtPolarPicker
{
public:
    MyPicker( QwtPolarCanvas *canvas ):
        QwtPolarPicker( canvas )
    {
        setStateMachine( new QwtPickerDragPointMachine() );
        setRubberBand( QwtPicker::NoRubberBand );
        setTrackerMode( AlwaysOn );
    }

    virtual QwtText trackerTextPolar( const QwtPointPolar &pos ) const
    {
        QColor bg( Qt::white );
        bg.setAlpha( 200 );

        QwtText text = QwtPolarPicker::trackerTextPolar( pos );
        text.setBackgroundBrush( QBrush( bg ) );
        return text;
    }
};
/**
*   @project         FrameDemo
*   @brief           极坐标类
*   @author          XuYuanbing
*   @data            2018-08-07
*/
class polar_Chart :public QwtPolarPlot
{
public:
    polar_Chart(QWidget *parent,
                double xdefMinValue,
                double xdefMaxValue,
                double ydefMinValue,
                double ydefMaxValue,
                double zdefMinValue,
                double zdefMaxValue
                ) : QwtPolarPlot( parent )
    {
        setAutoReplot( true );
        //setPlotBackground( Qt::darkBlue );

        setScale( QwtPolar::Azimuth,xdefMinValue, xdefMaxValue,90);
        //setScaleMaxMinor( QwtPolar::Azimuth, 2 );

        setScale( QwtPolar::AxisRight,ydefMinValue,ydefMaxValue,400);
        //setScaleMaxMinor( QwtPolar::Radius, 0 );

        polarGrid = new QwtPolarGrid();
        polarGrid->setPen( QPen( Qt::lightGray,1 ) );

//        polarGrid->setAxisPen( QwtPolar::AxisAzimuth, QPen( Qt::white ) );
//        polarGrid->setAxisPen( QwtPolar::AxisRight, QPen( Qt::red ) );
        //polarGrid->setAzimuthScaleDraw( new AzimuthScaleDraw() );
        //polarGrid->showAxis( QwtPolar::AxisAzimuth, true );
        polarGrid->showAxis( QwtPolar::AxisLeft, false );
        polarGrid->showAxis( QwtPolar::AxisRight, true );
        polarGrid->showAxis( QwtPolar::AxisTop, false );
        polarGrid->showAxis( QwtPolar::AxisBottom, false );
        polarGrid->showGrid( QwtPolar::Azimuth, true );
        polarGrid->showGrid( QwtPolar::Radius, true );
        polarGrid->attach( this );

        polarSpectrogram = new QwtPolarSpectrogram();
        polarSpectrogram->setPaintAttribute(QwtPolarSpectrogram::ApproximatedAtan, true );
        polarSpectrogram->setRenderThreadCount( 0 ); // use multi threading

        matrixRasterData = new QwtMatrixRasterData();
        matrixRasterData->setInterval( Qt::XAxis, QwtInterval( xdefMinValue, xdefMaxValue) );
        matrixRasterData->setInterval( Qt::YAxis, QwtInterval( ydefMinValue, ydefMaxValue) );
        matrixRasterData->setInterval( Qt::ZAxis, QwtInterval( zdefMinValue, zdefMaxValue) );
        polarSpectrogram->setData( matrixRasterData );
        polarSpectrogram->attach( this );
        polarSpectrogram->setZ( 1.0 );
        polarGrid->setZ( 1.0 );
        polarSpectrogram->setColorMap( new qwt_colormap(0) );
        QwtPolarPicker *picker = new MyPicker( canvas() );
        picker->setMousePattern( QwtEventPattern::MouseSelect1, Qt::RightButton );

        //new QwtPolarPanner( canvas() );
    }

    void setDataValue(QVector<double> vectorData,int iColumn)
    {
        matrixRasterData->setValueMatrix(vectorData,iColumn);
//        for(int j=0;j<60;j++)
//        {
//            for(int i =0;i<800;i++)
//            {
//                matrixRasterData->setValue(i,j,80);
//                //qDebug()<<vectorData[i];
//            }
//        }

        qDebug()<<matrixRasterData->numColumns();
        qDebug()<<matrixRasterData->numRows();
        polarSpectrogram->setData(matrixRasterData);
        replot();
    }

    QwtPolarGrid *returnPolarGrid()
    {
        return polarGrid;
    }

    QwtPolarSpectrogram *returnPolarSpectrogram()
    {
        return polarSpectrogram;
    }

    QwtMatrixRasterData *returnMatrixRasterData()
    {
        return matrixRasterData;
    }

private:
    QwtPolarGrid *polarGrid;
    QwtPolarSpectrogram *polarSpectrogram;
    QwtMatrixRasterData *matrixRasterData;
};

Qwt_Polar_Chart::Qwt_Polar_Chart(QWidget *parent,
                                 double xdefMinValue,
                                 double xdefMaxValue,
                                 double ydefMinValue,
                                 double ydefMaxValue,
                                 double zdefMinValue,
                                 double zdefMaxValue
                                 ) : QWidget( parent )
{
    polarChart = new polar_Chart(this,
                                 xdefMinValue,
                                 xdefMaxValue,
                                 ydefMinValue,
                                 ydefMaxValue,
                                 zdefMinValue,
                                 zdefMaxValue);
    QwtScaleWidget * scaleWidget = new QwtScaleWidget();
    scaleWidget->setAlignment( QwtScaleDraw::RightScale );
    scaleWidget->setColorBarEnabled( true );

    const QwtInterval interval =
       polarChart->returnPolarSpectrogram()->data()->interval( Qt::ZAxis );

    scaleWidget->setColorMap( interval, new qwt_colormap(0) );

//    QwtText title( "Intensity" );
//    scaleWidget->setTitle( title );

    QwtLinearScaleEngine scaleEngine;
    scaleWidget->setScaleDiv(scaleEngine.divideScale( interval.minValue(),
                                                      interval.maxValue(), 8, 5 ) );

//    int startDist, endDist;
//    scaleWidget->getBorderDistHint( startDist, endDist );
//    scaleWidget->setBorderDist( startDist, endDist );

    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( polarChart, 10 );
    layout->addWidget( scaleWidget, 10 );
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置数据
*   @author          XuYuanbing
*   @data            2018-08-07
*/
void Qwt_Polar_Chart::setDataValue(QVector<double> vectorData,int iColumn)
{
    polarChart->setDataValue(vectorData,iColumn);
    return;
}

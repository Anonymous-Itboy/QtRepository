#ifndef QWT_POLAR_CHART_H
#define QWT_POLAR_CHART_H

#include <qwt_polar_plot.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_polar_grid.h>
#include <qwt_polar_spectrogram.h>
#include <qwt_matrix_raster_data.h>

class polar_Chart;
class Qwt_Polar_Chart : public QWidget
{
public:
    Qwt_Polar_Chart(QWidget * = NULL,
                    double xdefMinValue = 0,
                    double xdefMaxValue = 360,
                    double ydefMinValue = 0,
                    double ydefMaxValue = 1,
                    double zdefMinValue = 0,
                    double zdefMaxValue = 1);
    void setDataValue(QVector<double> vectorData,int iColumn);

private:
//    QwtPolarGrid *polarGrid;
//    QwtPolarSpectrogram *polarSpectrogram;
//    QwtMatrixRasterData *matrixRasterData;
    polar_Chart *polarChart;

};

#endif // QWT_POLAR_CHART_H

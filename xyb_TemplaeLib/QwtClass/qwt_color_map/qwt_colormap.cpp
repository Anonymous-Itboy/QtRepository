#include "QwtClass/qwt_color_map/qwt_colormap.h"
#include <qwt_color_map.h>
#include <qwt_scale_widget.h>

qwt_colormap::qwt_colormap(int type)
             :QwtLinearColorMap( QColor(255,255,255),
                                 QColor(255,0,0),
                                 QwtColorMap::RGB )
{
    switch (type) {
    case 0:
    {
        addColorStop( 0.1, QColor(255,255,255) );
        addColorStop( 0.2, QColor(0,255,255) );
        addColorStop( 0.3, QColor(0,127,255) );
        addColorStop( 0.8, QColor(255,150,0) );
        addColorStop( 0.9, QColor(255,100,0) );
        addColorStop( 1.0, QColor(225,0,0) );
        break;
    }
    default:
        break;
    }

    return;
}

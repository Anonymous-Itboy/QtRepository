#-------------------------------------------------
#
# Project created by QtCreator 2018-12-18T10:12:40
#
#-------------------------------------------------

QT       += core gui
#-----communication--------
QT       += network

#-----charts---------------
QT       += charts

#-----play-----------------
QT       += multimedia

#-----sql------------------
QT       += sql

##---------qwt-----------------------------------------
DEFINES  += QT_DLL QWT_DLL QWT_POLAR_DLL
CONFIG   += qwt
CONFIG   += qwtpolar
#build debug version need lib
LIBS += -L"V:\SmartGitWorkSpace\Qt\xyb_TemplaeLib\QwtClass\include\qwt\debug\lib" -lqwtd
LIBS += -L"V:\SmartGitWorkSpace\Qt\xyb_TemplaeLib\QwtClass\include\qwtpolar\debug\lib" -lqwtpolard

#build release version need lib
#LIBS += -L"V:\SmartGitWorkSpace\Qt\xyb_TemplaeLib\QwtClass\include\qwt\release\lib" -lqwt
#LIBS += -L"V:\SmartGitWorkSpace\Qt\xyb_TemplaeLib\QwtClass\include\qwtpolar\release\lib" -lqwtpolar

INCLUDEPATH += V:\SmartGitWorkSpace\Qt\xyb_TemplaeLib\QwtClass\include\qwt\src
INCLUDEPATH += V:\SmartGitWorkSpace\Qt\xyb_TemplaeLib\QwtClass\include\qwtpolar\src

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xyb_TemplaeLib
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    CommuniCation/FTP/ftpclient.cpp \
    CommuniCation/UDP/session_udp.cpp \
    GroupUi/groupComboBox/groupcombobox.cpp \
    GroupUi/groupLineEdit/grouplineedit.cpp \
    GroupUi/groupPolarChart/grouppolarchart.cpp \
    GroupUi/groupSoundPro/groupsoundpro.cpp \
    GroupUi/groupWavPlay/analyzefile.cpp \
    GroupUi/groupWavPlay/chartview.cpp \
    GroupUi/groupWavPlay/cwavefile.cpp \
    GroupUi/groupWavPlay/groupwavplay.cpp \
    GroupUi/groupWorkStatus/groupworkstatus.cpp \
    logMessage/logmessage.cpp \
    QwtClass/qwt_color_map/qwt_colormap.cpp \
    QwtClass/qwt_polar_chart/qwt_polar_chart.cpp \
    QwtClass/qwt_xy_chart/qwt_xy_chart.cpp \
    dataBase/dataBase.cpp

HEADERS += \
    mainwindow.h \
    CommuniCation/FTP/ftpclient.h \
    CommuniCation/UDP/session_udp.h \
    GroupUi/groupComboBox/groupcombobox.h \
    GroupUi/groupLineEdit/grouplineedit.h \
    GroupUi/groupPolarChart/grouppolarchart.h \
    GroupUi/groupSoundPro/groupsoundpro.h \
    GroupUi/groupWavPlay/analyzefile.h \
    GroupUi/groupWavPlay/chartview.h \
    GroupUi/groupWavPlay/cwavefile.h \
    GroupUi/groupWavPlay/groupWavPlay.h \
    GroupUi/groupWorkStatus/groupworkstatus.h \
    logMessage/logmessage.h \
    QwtClass/qwt_color_map/qwt_colormap.h \
    QwtClass/qwt_polar_chart/qwt_polar_chart.h \
    QwtClass/qwt_xy_chart/qwt_xy_chart.h \
    dataBase/dataBase.h

FORMS += \
    mainwindow.ui \
    GroupUi/groupSoundPro/groupsoundpro.ui \
    GroupUi/groupWavPlay/groupWavPlay.ui

RESOURCES += \
    src/src.qrc

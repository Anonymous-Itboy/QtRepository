#ifndef GROUPWORKSTATUS_H
#define GROUPWORKSTATUS_H

#include <QLabel>
#include <QWidget>
#include <QTimer>

class groupWorkStatus : public QWidget
{
    Q_OBJECT
public:
    groupWorkStatus(int timeSec = 1) ;
    void updateRunningStatus(bool status);

public slots:
    void setRunningStop();

private:
    int itimeSec;
    QLabel *lable;
    QTimer *setBtnTimer;
    unsigned int  uiheartBeat;                 //心跳计数
};

#endif // GROUPWORKSTATUS_H

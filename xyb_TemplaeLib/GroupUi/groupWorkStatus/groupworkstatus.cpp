/**
*   @project         FrameDemo
*   @brief           运行状态
*   @author          XuYuanbing
*   @data            2018-08-09
*/
#include <QLabel>
#include <QObject>
#include <QHBoxLayout>

#include "groupworkstatus.h"

groupWorkStatus::groupWorkStatus(int timeSec) : QWidget()
{
    itimeSec = timeSec;
    setBtnTimer = new QTimer(this);
    uiheartBeat = 0;
    lable = new QLabel(this);
    setRunningStop();
    lable->setMinimumSize(20,20);
    lable->show();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(lable);
    this->setLayout(layout);
    connect(setBtnTimer,SIGNAL(timeout()),this,SLOT(setRunningStop()));

}
//更新参数状态
void groupWorkStatus::updateRunningStatus(bool status)
{
    if (status)
    {
        switch (uiheartBeat%2)
        {
            case 0:
            {
                lable->setStyleSheet("image: url(:/img/NormalDot.png);");
                break;
            }
            case 1:
            {
                lable->setStyleSheet("image: url(:/img/RunningDot.png);");
                break;
            }
            default:
                break;
        }
        uiheartBeat += 1;
        setBtnTimer->stop();
    }else {
        lable->setStyleSheet("image: url(:/img/ErrorDot.png);");
    }
    setBtnTimer->start(1000*itimeSec);
    return;
}
//设置运行停止运行标志
void groupWorkStatus::setRunningStop()
{
    updateRunningStatus(true);
    //lable->setStyleSheet("image: url(:/img/ErrorDot.png);");
    return;
}

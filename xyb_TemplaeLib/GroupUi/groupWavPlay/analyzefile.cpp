#include "analyzefile.h"
#include <QDebug>
using namespace std;

analyzeFile::analyzeFile()
{
    wavFile = new CWaveFile();
}
/**
*   @project         FrameDemo
*   @brief           摘要
*   @author          XuYuanbing
*   @data            2018-11-22
*/
analyzeFile::~analyzeFile()
{

}
/**
*   @project         FrameDemo
*   @brief           线程run函数
*   @author          XuYuanbing
*   @data            2018-11-22
*/
void analyzeFile::run()
{
    wavFile->FileOpen(filePath.toStdString().c_str());

    std::vector<int> vecDatas;
    wavFile->setScalingFactor(20);
    wavFile->GetDatas(vecDatas);

    int iSize = vecDatas.size();
    int max = 0, min = 0;
    int iSimple = wavFile->getSimple();
    int iUnitSize = iSize/100;
    int iNowSize  = 0;
    int iBarValue = 0;
    emit setVoiceChartXmax(iSize*1.0/iSimple);
    wavFile->FileClose();
    for(int i = 0; i < iSize; i++)
    {
        if(i == 0)
        {
            max = min = vecDatas[i];
        }
        if(max < vecDatas[i])
        {
            max = vecDatas[i];
        }
        if(min > vecDatas[i])
        {
            min = vecDatas[i];
        }
    }
//    for(int i = 0; i < iSize; i++)
//    {
//       qDebug()<<"x:"<<i*1.0/iSimple<<"y:"<<(1-(-1))*(vecDatas[i]-min)/(max-min+1e-8)+(-1);

//       sleep(1);
//    }
    for(int i = 0; i < iSize; i++)
    {
        emit addVoiceSeriesPoint(i*1.0/iSimple,(1-(-1))*(vecDatas[i]-min)/(max-min+1e-8)+(-1));
        if(iNowSize == i)
        {
            iNowSize += iUnitSize;
            emit setBarValue(iBarValue);
            iBarValue += 1;
        }
        if(i + 1 == iSize)
        {
            emit analyzeFileFinish();
        }
        usleep(10);
    }

//    while (1) {
//        qDebug()<<"x:----";
//        sleep(1);
//    }
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置文件路径
*   @author          XuYuanbing
*   @data            2018-11-22
*/
void analyzeFile::setFilePath(const QString path)
{
    if(!path.isEmpty())
    {
        filePath = path;
    }
    qDebug()<<path;
    return;
}

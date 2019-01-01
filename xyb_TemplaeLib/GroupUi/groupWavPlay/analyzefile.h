#ifndef ANALYZEFILE_H
#define ANALYZEFILE_H

#include <QThread>
#include "cwavefile.h"

class analyzeFile : public QThread
{
    Q_OBJECT
public:
    analyzeFile();
    ~analyzeFile();

    void run();
    void setFilePath(const QString path);

private:
    CWaveFile *wavFile;
    QString  filePath;

signals:
    void setBarValue(int);
    void setVoiceChartXmax(int);
    void addVoiceSeriesPoint(qreal, qreal);
    void analyzeFileFinish();
};

#endif // ANALYZEFILE_H

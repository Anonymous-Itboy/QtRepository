#ifndef GROUPWAVPLAY_H
#define GROUPWAVPLAY_H

#include <QWidget>
#include <QChartGlobal>
#include <QAudio>
#include <QFile>
#include <QTimer>
#include <QChartView>
#include <QMediaPlayer>

#include "chartview.h"
#include "cwavefile.h"
#include "analyzefile.h"

namespace Ui {
class groupWavPlay;
}

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class XYSeriesIODevice;

QT_BEGIN_NAMESPACE
class QAudioOutput;
class QAudioDecoder;
QT_END_NAMESPACE

class groupWavPlay : public QWidget
{
    Q_OBJECT

public:
    explicit groupWavPlay(QWidget *parent = 0);
    ~groupWavPlay();

    void setEnvelopeSpecData(const QString &);

    void setWinClose();
    void clearVoiceSeries();
    void addToPlaylist(const QList<QUrl> urls);

private:
    void initUi();
    void initSignal();
    void initVoiceChart();

public slots:
    void onPlaySeries(int);

private slots:
    void on_playBtn_clicked();
    void on_stopBtn_clicked();
    void updataBarValue(int);
    void updateDurationInfo(qint64 currentInfo);
    void seek(int seconds);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void addVoiceSeriesPoint(qreal x, qreal y);
    void setVoiceChartXmax(int value);
    void analyzeFileFinish();

private:
    Ui::groupWavPlay *ui;

    QChart *m_voiceChart;
    ChartView *chartView;
    int ivoiceChartXmax;
    int ivoiceChartYmin;
    int ivoiceChartYmax;

    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    qint64 duration;

    analyzeFile *wavFile;
    QLineSeries *m_voiceSeries;
    QLineSeries *m_playSeries;
    QAudioOutput *m_audioOutput;

    QFile m_sourceFile;
    QTimer *m_timer;
    bool playState;
    bool initVoiceCharts;

    int  icutStart;
    int  icutEnd;

signals:
    void setBarValue(int);
};

#endif // GROUPWAVPLAY_H

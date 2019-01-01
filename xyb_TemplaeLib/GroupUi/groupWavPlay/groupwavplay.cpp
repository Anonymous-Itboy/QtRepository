#include "groupWavPlay.h"
#include "ui_groupWavPlay.h"

#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QAudioDecoder>
#include <QAudioBuffer>
#include <QMediaPlayer>
#include <QMediaPlayList>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QFileInfo>

#include "cwavefile.h"
#include "analyzefile.h"

QT_CHARTS_USE_NAMESPACE

groupWavPlay::groupWavPlay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::groupWavPlay),
    playState(false)
{
    ui->setupUi(this);
    wavFile = new analyzeFile();
    ivoiceChartYmin = -1;
    ivoiceChartYmax = 1;
    initUi();
    initSignal();

}

groupWavPlay::~groupWavPlay()
{
    delete m_voiceChart;
    delete m_voiceSeries;
    delete m_playSeries;

    if(m_sourceFile.isOpen())
    {
        m_sourceFile.close();
    }
    delete ui;
}

void groupWavPlay::setEnvelopeSpecData(const QString & name)
{
    m_playSeries->clear();
    m_voiceSeries->clear();

    wavFile->setFilePath(name);
    wavFile->start();

//    wavFile->FileOpen(name.toStdString().c_str());

//    std::vector<int> vecDatas;
//    wavFile->setScalingFactor(20);
//    wavFile->GetDatas(vecDatas);

//    int iSize = vecDatas.size();
//    int iSimple = wavFile->getSimple();
//    int iBarValue = 0;
//    int iUnitSize = iSize/100;
//    int iNowSize  = 0;

//    ui->horizontalSlider->setMinimum(0);
//    ui->horizontalSlider->setMaximum(iSize);
//    ui->progressBar->setValue(iBarValue);
//    if(!initVoiceCharts)
//    {
//        m_voiceSeries->hide();
//    }
//    wavFile->FileClose();
//    for(int i = 0; i < iSize; i++)
//    {
//        if(i == 0)
//        {
//            max = min = vecDatas[i];
//        }
//        if(max < vecDatas[i])
//        {
//            max = vecDatas[i];
//        }
//        if(min > vecDatas[i])
//        {
//            min = vecDatas[i];
//        }
//    }
//    for(int i = 0; i < iSize; i++)
//    {

//        m_voiceSeries->append(i*1.0/iSimple,(1-(-1))*(vecDatas[i]-min)/(max-min+1e-8)+(-1));
//        if(iNowSize == i)
//        {
//            iNowSize += iUnitSize;
//            emit setBarValue(iBarValue);
//            iBarValue += 1;
//        }
//        if(i + 1 == iSize)
//        {
//            m_voiceSeries->show();
//            initVoiceCharts = true;
//            ui->progressBar->setValue(100);
//            ui->playBtn->setEnabled(true);
//            ui->stopBtn->setEnabled(true);
//        }
//    }


    m_playSeries->append(0,-1);
    m_playSeries->append(0,1);

    QList<QUrl> urls;
    urls.append(QUrl(name));
    addToPlaylist(urls);
    player->setVolume(60);

}
void groupWavPlay::addVoiceSeriesPoint(qreal x, qreal y)
{
    m_voiceSeries->append(x,y);
    return;
}

void groupWavPlay::setVoiceChartXmax(int value)
{
    ivoiceChartXmax = value;
    m_voiceChart->axisX()->setRange(0, ivoiceChartXmax);
    this->duration  = ivoiceChartXmax;
    chartView->setCutStartTime(0);
    chartView->setCutEndTime(ivoiceChartXmax);
    ui->horizontalSlider->setRange(0, this->duration);
    updateDurationInfo(this->duration / 1000);
    return;
}
void groupWavPlay::analyzeFileFinish()
{
    m_voiceSeries->show();
    initVoiceCharts = true;
    ui->progressBar->setValue(100);
    ui->playBtn->setEnabled(true);
    ui->stopBtn->setEnabled(true);
    wavFile->terminate();
}

static bool isPlaylist(const QUrl &url)
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void groupWavPlay::addToPlaylist(const QList<QUrl> urls)
{
    foreach (const QUrl &url, urls) {
        if (isPlaylist(url))
            playlist->load(url);
        else
            playlist->addMedia(url);
    }
}

void groupWavPlay::setWinClose()
{
    m_sourceFile.close();
    this->close();
}

void groupWavPlay::initUi()
{
    initVoiceCharts = false;

    m_voiceChart  = new QChart();
    m_voiceSeries = new QLineSeries(this);
    m_playSeries  = new QLineSeries(this);
    m_timer = new QTimer();
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist();

    player->setPlaylist(playlist);
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    ui->horizontalSlider->setRange(0, 1);
    ui->playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->playBtn->setEnabled(false);
    ui->stopBtn->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->stopBtn->setEnabled(false);

    initVoiceChart();
    return;
}

void groupWavPlay::initSignal()
{
    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));
    connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(on_stopBtn_clicked()));

    connect(wavFile, SIGNAL(setBarValue(int)), this, SLOT(updataBarValue(int)));
    connect(wavFile, SIGNAL(setVoiceChartXmax(int)), this, SLOT(setVoiceChartXmax(int)));
    connect(wavFile, SIGNAL(addVoiceSeriesPoint(qreal, qreal)), this, SLOT(addVoiceSeriesPoint(qreal, qreal)));
    connect(wavFile, SIGNAL(analyzeFileFinish()), this, SLOT(analyzeFileFinish()));
    return;
}

void groupWavPlay::initVoiceChart()
{
    chartView = new ChartView(m_voiceChart);
    m_voiceChart->addSeries(m_voiceSeries);
    m_voiceChart->addSeries(m_playSeries);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 10);
    axisX->setLabelFormat("%d");
    axisX->setTickCount(11);
    axisX->setTitleText(QString::fromLocal8Bit("时间（s）"));
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-1, 1);
    axisY->setTickCount(3);
    axisY->setTitleText(QString::fromLocal8Bit("幅度"));

    m_voiceChart->setAxisX(axisX, m_voiceSeries);
    m_voiceChart->setAxisY(axisY, m_voiceSeries);
    m_voiceChart->setAxisX(axisX, m_playSeries);
    m_voiceChart->setAxisY(axisY, m_playSeries);

    m_voiceChart->legend()->hide();
    ui->horizontalLayout_3->addWidget(chartView);

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format))
    {
        qDebug() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    QFont font;
    font.setPixelSize(18);
    m_voiceChart->setTitleFont(font);
    m_voiceChart->setBackgroundBrush(QBrush(QColor(60,60,60)));

    m_voiceSeries->setPen(QPen(QColor(0,255,0),1));
    m_voiceSeries->setUseOpenGL(true);

    QPen pen;
    pen.setColor(QColor(255,60,60));
    pen.setWidth(2);
    m_playSeries->setPen(pen);
    m_playSeries->setUseOpenGL(true);

    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setBackgroundBrush(QBrush(QColor(60,60,60)));
    chartView->setRubberBand(QChartView::RectangleRubberBand);

    axisX->setLabelsColor(QColor("#DCDCDC"));
    axisY->setLabelsColor(QColor("#DCDCDC"));

    axisX->setTitleBrush(QBrush(QColor("#DCDCDC")));
    axisY->setTitleBrush(QBrush(QColor("#DCDCDC")));

    axisX->setTitleFont(QFont("微软雅黑", 10));
    axisY->setTitleFont(QFont("微软雅黑", 10));

     m_playSeries->hide();
     return;
}

void groupWavPlay::onPlaySeries(int x)
{
    if(x >= ivoiceChartXmax)
    {
        m_playSeries->clear();
        m_playSeries->append(0,ivoiceChartYmin);
        m_playSeries->append(0,ivoiceChartYmax);
        m_playSeries->hide();
        return;;
    }

    if(x > 0)
    {
        m_playSeries->clear();
        m_playSeries->append(x,ivoiceChartYmin);
        m_playSeries->append(x,ivoiceChartYmax);
        m_playSeries->show();
        m_voiceChart->update();
    }

    return;
}

void groupWavPlay::on_playBtn_clicked()
{
    icutStart = chartView->returnCutStartTime();
    icutEnd   = chartView->returnCutEndTime();
    if(icutStart < 0)
    {
        icutStart = 0;
    }
    if(0 >= icutEnd)
    {
        icutEnd = this->duration;
    }

    qDebug()<<"cutStart"<<icutStart<<"cutEnd"<<icutEnd <<this->duration;

    switch (player->state()) {
    case QMediaPlayer::StoppedState:
        if(icutEnd-icutStart > 0)
        {
            seek(icutStart);
            ui->horizontalSlider->setValue(icutStart);
            m_timer->start((icutEnd-icutStart+1)*1000);
        }
        else {
            return;
        }
        player->play();
        ui->stopBtn->setEnabled(true);
        ui->playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    case QMediaPlayer::PlayingState:
        player->pause();
        m_timer->stop();
        ui->stopBtn->setEnabled(true);
        ui->playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    case QMediaPlayer::PausedState:
        player->play();
        m_timer->start((icutEnd * 1000)- player->position());
        ui->stopBtn->setEnabled(true);
        ui->playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
    m_playSeries->show();
    return;
}

void groupWavPlay::on_stopBtn_clicked()
{
    player->stop();
    m_timer->stop();
//    m_voiceChart->zoomReset();

    if(icutStart == 0 && icutEnd == this->duration)
    {
        m_voiceChart->axisX()->setRange(0, ivoiceChartXmax);
        m_voiceChart->axisY()->setRange(ivoiceChartYmin, ivoiceChartYmax);
    }

    m_playSeries->clear();
    m_playSeries->append(0,ivoiceChartYmin);
    m_playSeries->append(0,ivoiceChartYmax);
    m_playSeries->hide();
    ui->playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->stopBtn->setEnabled(false);
    return;
}

void groupWavPlay::updataBarValue(int value)
{
    ui->progressBar->setValue(value);
    return;
}
void groupWavPlay::clearVoiceSeries()
{
    m_voiceSeries->clear();
    return;
}

/*
    播放相关
*/
void groupWavPlay::seek(int seconds)
{
    player->setPosition(seconds * 1000);
    onPlaySeries(seconds);
    return;
}
void groupWavPlay::durationChanged(qint64 duration)
{
    if(duration > 0)
    {
        this->duration = duration/1000;
        ui->horizontalSlider->setMaximum(duration / 1000);
    }
    return;
}
void groupWavPlay::positionChanged(qint64 progress)
{
    if (!ui->horizontalSlider->isSliderDown())
    {
        ui->horizontalSlider->setValue(progress / 1000);
        onPlaySeries(progress / 1000);
        updateDurationInfo(progress / 1000);
    }
    return;

}
void groupWavPlay::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || duration)
    {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    ui->label->setText(tStr);
    return;
}

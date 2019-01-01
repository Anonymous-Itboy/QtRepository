#ifndef GROUPSOUNDPRO_H
#define GROUPSOUNDPRO_H

#include <QWidget>
#include <QString>
#include <QVector>
#include <QChart>
#include <QValueAxis>
#include <QSplineSeries>
#include <QStandardItemModel>

namespace Ui {
class groupSoundPro;
}
using namespace QtCharts;
class groupSoundPro : public QWidget
{
    Q_OBJECT

public:
    explicit groupSoundPro(QWidget *parent = 0,const QString name = nullptr);
    ~groupSoundPro();

typedef struct soundpro_data_st
{
    unsigned short  ucId;
    unsigned short  ucDepth;
    unsigned short  ucSpeed;
}SOUNDPRO_DATA_S;
typedef struct model_soundpro_st
{
    unsigned short  ucTypeId;
    unsigned short  ucSoundProNum;
    SOUNDPRO_DATA_S stData[300];
}MODEL_SOUNDPRO_S;

    int getSoundProFrom();
    int getSoundProType();
    int getSoundProNum();
    QVector<SOUNDPRO_DATA_S> getSoundProData();

private:

    Ui::groupSoundPro *ui;
    QString configJsonName;
    unsigned short usMaxSoundProNum;    //最大声剖层数
    unsigned short usEffectiveSoundProNum;
    MODEL_SOUNDPRO_S usaModelSoundProData;
    QVector<MODEL_SOUNDPRO_S> vtModelSound;
    QChart *pChart;
    QValueAxis *paxisX;
    QValueAxis *paxisY;
    QSplineSeries *pserVer;
    QStandardItemModel * envSndPrfCntModel;             //输入声速剖面表格model

    void readConfigJson();
    void initSignal();
    void initChart();
    void initDataView();

private slots:
    void changeDataFrom(int);
    void changeType(int);
    void changeShowView(int);
    void changeShoundProNum(int);
};

#endif // GROUPSOUNDPRO_H

/**
*   @project         FrameDemo
*   @brief           声速剖面组合UI
*   @author          XuYuanbing
*   @data            2018-08-01
*/
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QChartView>

#include "groupsoundpro.h"
#include "ui_groupsoundpro.h"

groupSoundPro::groupSoundPro(QWidget *parent,const QString path) :
    QWidget(parent),
    ui(new Ui::groupSoundPro)
{
    ui->setupUi(this);
    configJsonName = "save.json";
    readConfigJson();
    ui->pcmDataFrom->setTitle(QString::fromLocal8Bit("来源"));
    ui->pcmSndProType->setTitle(QString::fromLocal8Bit("类型"));
    ui->pcmSndProNum->setTitle(QString::fromLocal8Bit("层数"));
    ui->pcmShowView->setTitle(QString::fromLocal8Bit("显示"));
    ui->pcmShowView->addItem(QString::fromLocal8Bit("曲线"));
    ui->pcmShowView->addItem(QString::fromLocal8Bit("数据"));
    ui->tabWidget_4->tabBar()->hide();


    initSignal();
    initChart();
    initDataView();
    changeDataFrom(0);
    return;
}

groupSoundPro::~groupSoundPro()
{
    delete ui;
}
/**
*   @project         FrameDemo
*   @brief           初始化事件
*   @author          XuYuanbing
*   @data            2018-08-02
*/
void groupSoundPro::initSignal()
{
    connect(ui->pcmDataFrom, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeDataFrom(int)));                   //声剖来源选择
    connect(ui->pcmSndProType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeType(int)));                       //声剖类型选择
    connect(ui->pcmSndProNum, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeShoundProNum(int)));               //声剖层数选择
    connect(ui->pcmShowView, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeShowView(int)));                   //显示视图选择
}

/**
*   @project         FrameDemo
*   @brief           读取配置文件
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupSoundPro::readConfigJson()
{
    QFile file(configJsonName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"read json file failed";
        return;
    }
    QByteArray ba = file.readAll();
    //qDebug()<<ba;
    file.close();

    QJsonDocument jsonD = QJsonDocument::fromJson(QString::fromLocal8Bit(ba.data()).toStdString().data());
    //qDebug()<<jsonD;
    QJsonObject rootObj = jsonD.object();
    //qDebug()<<rootObj;
    QStringList rootKeys = rootObj.keys();

    QStringList fromItems;
    QStringList typeItems;

    QJsonObject fromObj;
    QJsonObject typeObj;
    QJsonObject soundProObj;
    QJsonObject onesoundProObj;

    for(int rootId=0; rootId < rootKeys.size();rootId++)
    {
        //qDebug()<<"rootKeys "<<rootId<<" is:"<<rootKeys.at(rootId);
        fromObj = rootObj.value(rootKeys.at(rootId)).toObject();
        fromItems << fromObj["name"].toString();
//        qDebug()<<"name:"<<subObj["name"].toString();

        if(0 == rootId)
        {
            QStringList fromKeys = fromObj.keys();
            for(int fromId =0;fromId < fromKeys.size();fromId++)
            {
                //qDebug()<<"fromKeys "<<fromId<<" is:"<<fromKeys.at(fromId);
                if(fromId > 0)
                {
                    typeObj = fromObj.value(fromKeys.at(fromId)).toObject();
                    typeItems << typeObj["name"].toString();
                    QStringList typeKeys = typeObj.keys();
                    for(int typeId=0;typeId< typeKeys.size();typeId++)
                    {
                        //qDebug()<<"typeKeys "<<typeId<<" is:"<<typeKeys.at(typeId);
                        if(typeId > 0)
                        {
                            soundProObj = typeObj.value(typeKeys.at(typeId)).toObject();
                            QStringList soundProKeys = soundProObj.keys();
                            for(int soundProId=0;soundProId< soundProKeys.size();soundProId++)
                            {
                                //qDebug()<<"soundProKeys "<<soundProId<<" is:"<<soundProKeys.at(soundProId);
                                if(soundProId > 0)
                                {
                                    onesoundProObj = soundProObj.value(soundProKeys.at(soundProId)).toObject();
                                    //qDebug()<<"depth:"<<onesoundProObj["depth"].toString();
                                    //qDebug()<<"speed:"<<onesoundProObj["speed"].toString();

                                    usaModelSoundProData.ucTypeId       = typeId;
                                    usaModelSoundProData.ucSoundProNum  = soundProObj["num0"].toString().toShort();
                                    usaModelSoundProData.stData[soundProId-1].ucId    = soundProId;
                                    usaModelSoundProData.stData[soundProId-1].ucDepth = onesoundProObj["depth"].toString().toShort();
                                    usaModelSoundProData.stData[soundProId-1].ucSpeed = onesoundProObj["speed"].toString().toShort();
                                }
                            }
                            vtModelSound.push_back(usaModelSoundProData);
                        }
                    }
                }
            }
        }
        if(1 == rootId)
        {
            usMaxSoundProNum = fromObj["maxSoundProNum"].toString().toShort();
        }
    }
    ui->pcmDataFrom->addItems(fromItems);
    ui->pcmSndProType->addItems(typeItems);
    QStringList maxSoundProItems;
    for(int i = 0;i <= usMaxSoundProNum;i++)
    {
        maxSoundProItems << QString::number(i);
    }
    ui->pcmSndProNum->addItems(maxSoundProItems);

    return;
}
/**
*   @project         FrameDemo
*   @brief           数据来源选择
*   模型输入时：类型下拉框可选择  声剖层数不可点击
*   人工输入时：类型选择不可点击  声剖层数下拉框可选择
*   其他情况下：类型选择不可点击  声剖层数不可点击
*   @author          XuYuanbing
*   @data            2018-08-02
*/
void groupSoundPro::changeDataFrom(int iType)
{
    switch (iType) {
    case 0:
    {
        ui->pcmSndProType->setStatusEdit(true);
        ui->pcmSndProNum->setStatusEdit(false);
        ui->pcmShowView->setStatusEdit(true);
        ui->tabWidget_4->tabBar()->setCurrentIndex(0);
        ui->pcmShowView->setCurrentIndex(0);
        ui->pcmSndProType->setCurrentIndex(0);
        changeType(0);
        break;
    }
    case 1:
    {
        ui->pcmSndProType->setStatusEdit(false);
        ui->pcmSndProNum->setStatusEdit(true);
        ui->pcmShowView->setStatusEdit(true);
        ui->tabWidget_4->tabBar()->setCurrentIndex(1);
        ui->pcmShowView->setCurrentIndex(1);
        ui->pcmSndProNum->setCurrentIndex(0);
        ui->ptableViewpchartSoundPro->setEditTriggers(QAbstractItemView::DoubleClicked);          //设置双击被编辑
        break;
    }
    default:
        ui->pcmSndProNum->setCurrentIndex(0);
        ui->pcmSndProType->setStatusEdit(false);
        ui->pcmSndProNum->setStatusEdit(false);
        ui->pcmShowView->setStatusEdit(false);
        ui->ptableViewpchartSoundPro->setEditTriggers(QAbstractItemView::NoEditTriggers);          //设置不能被编辑
        break;
    }
    return;
}
/**
*   @project         FrameDemo
*   @brief           数据类型选择
*   @author          XuYuanbing
*   @data            2018-08-02
*/
void groupSoundPro::changeType(int iType)
{
    ui->pcmSndProNum->setCurrentIndex(vtModelSound[iType].ucSoundProNum);
    pserVer->clear();
    envSndPrfCntModel->removeRows(0,envSndPrfCntModel->rowCount());
    QList<QStandardItem *> listRow;
    int type = iType;
    for(int j =0;j< vtModelSound[type].ucSoundProNum;j++)
    {
        *pserVer << QPointF(vtModelSound[type].stData[j].ucSpeed,
                            vtModelSound[type].stData[j].ucDepth);
        listRow.clear();
        listRow.append(new QStandardItem( QString::number(vtModelSound[type].stData[j].ucDepth)));
        listRow.append(new QStandardItem( QString::number(vtModelSound[type].stData[j].ucSpeed)));
        int count = listRow.count();
        for(int i = 0; i < count; i++)
        {
            listRow.at(i)->setTextAlignment(Qt::AlignCenter);
        }
        envSndPrfCntModel->appendRow(listRow);
        envSndPrfCntModel->item(j,0)->setForeground(QBrush(QColor(0,225,0)));
        envSndPrfCntModel->item(j,1)->setForeground(QBrush(QColor(0,225,0)));
    }

    return;
}
/**
*   @project         FrameDemo
*   @brief           初始化声剖曲线坐标系
*   @author          XuYuanbing
*   @data            2018-08-02
*/
void groupSoundPro::initChart()
{
    paxisX  = new QValueAxis();
    paxisY  = new QValueAxis();
    pChart  = new QChart();
    pserVer = new QSplineSeries();

//    paxisX->setTitleBrush(QBrush(QColor(0, 255, 0)));
    paxisX->setTitleText(QString::fromLocal8Bit("传播速度(m/s)"));
//    paxisX->setGridLineColor(QColor("#606060"));                        //设置网格线颜色
//    paxisX->setLabelsColor(QColor(0, 255, 0));                          //设置x轴字体颜色
    paxisX->setRange(1400, 1600);
    paxisX->setLabelFormat("%d");
    paxisX->setTickCount(4);

//    paxisY->setTitleBrush(QBrush(QColor(0, 255, 0)));
    paxisY->setTitleText(QString::fromLocal8Bit("深度(m)"));
//    paxisY->setGridLineColor(QColor("#606060"));                        //设置网格线颜色
//    paxisY->setLabelsColor(QColor(0, 255, 0));                          //设置y轴字体颜色
    paxisY->setReverse(true);                                           //设置y轴正方向向下
    paxisY->setLabelFormat("%d");
    //paxisY->setRange(0, 7000);

    pChart->addAxis(paxisX, Qt::AlignTop);
    pChart->addAxis(paxisY, Qt::AlignLeft);
//    pChart->setBackgroundBrush(QBrush(QColor("#484848")));
    pChart->setMargins(QMargins(0,0,0,0));
    pChart->legend()->hide();

    for(int j =0;j< vtModelSound[0].ucSoundProNum;j++)
    {
        *pserVer << QPointF(vtModelSound[0].stData[j].ucSpeed,
                            vtModelSound[0].stData[j].ucDepth);
    }
    pChart->addSeries(pserVer);
    pserVer->attachAxis(paxisX);
    pserVer->attachAxis(paxisY);
    pserVer->show();

    ui->pchartSoundPro->setChart(pChart);
    ui->pchartSoundPro->setRenderHint(QPainter::Antialiasing);
    ui->pchartSoundPro->update();
}
/**
*   @project         FrameDemo
*   @brief           改变显示事件
*   @author          XuYuanbing
*   @data            2018-08-03
*/
void groupSoundPro::changeShowView(int iType)
{
    switch (iType) {
    case 0:
    {
        ui->tabWidget_4->tabBar()->setCurrentIndex(0);
        if(ui->pcmDataFrom->getComboIndex() == 1)
        {
            pserVer->clear();
            usEffectiveSoundProNum = ui->pcmSndProNum->getComboIndex();
            for(int i = 0; i < usEffectiveSoundProNum;i++)
            {
                *pserVer << QPointF(envSndPrfCntModel->item(i,1)->text().toInt(),
                                    envSndPrfCntModel->item(i,0)->text().toInt());
            }
        }
        break;
    }
    case 1:
    {
        ui->tabWidget_4->tabBar()->setCurrentIndex(1);
        break;
    }
    default:
        break;
    }
}
/**
*   @project         FrameDemo
*   @brief           初始化数据表视图
*   @author          XuYuanbing
*   @data            2018-08-03
*/
void groupSoundPro::initDataView()
{
    envSndPrfCntModel = new QStandardItemModel();
    QStringList list;
    list << QString::fromLocal8Bit("深度（m）")
         << QString::fromLocal8Bit("速度（m/s）");
    envSndPrfCntModel->clear();
    envSndPrfCntModel->setHorizontalHeaderLabels(list);

    ui->ptableViewpchartSoundPro->setModel(envSndPrfCntModel);
    ui->ptableViewpchartSoundPro->setAlternatingRowColors(true);
    ui->ptableViewpchartSoundPro->setShowGrid(false);
    ui->ptableViewpchartSoundPro->setDragEnabled(false);
    ui->ptableViewpchartSoundPro->setMouseTracking(true);
    ui->ptableViewpchartSoundPro->setFocusPolicy(Qt::NoFocus);
    ui->ptableViewpchartSoundPro->verticalHeader()->setVisible(false);
    ui->ptableViewpchartSoundPro->horizontalHeader()->setStretchLastSection(true);
    ui->ptableViewpchartSoundPro->horizontalHeader()->setHighlightSections(false);
    ui->ptableViewpchartSoundPro->horizontalHeader()->setMaximumHeight(30);
    ui->ptableViewpchartSoundPro->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ptableViewpchartSoundPro->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ptableViewpchartSoundPro->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->ptableViewpchartSoundPro->setEditTriggers(QAbstractItemView::NoEditTriggers);          //设置不能被编辑

}
/**
*   @project         FrameDemo
*   @brief           人工输入声剖层数选择
*   @author          XuYuanbing
*   @data            2018-08-03
*/
void groupSoundPro::changeShoundProNum(int num)
{
    if(num > 0 && ui->pcmDataFrom->getComboIndex() == 1)
    {
        pserVer->clear();
        envSndPrfCntModel->removeRows(0,envSndPrfCntModel->rowCount());
        QList<QStandardItem *> listRow;
        for(int i =0 ;i < num ;i++)
        {
            listRow.clear();
            listRow.append(new QStandardItem( QString::fromLocal8Bit("0")));
            listRow.append(new QStandardItem( QString::fromLocal8Bit("0")));

            int count = listRow.count();
            for(int i = 0; i < count; i++)
            {
                listRow.at(i)->setTextAlignment(Qt::AlignCenter);
            }
            envSndPrfCntModel->appendRow(listRow);
            envSndPrfCntModel->item(i,0)->setForeground(QBrush(QColor(0,225,0)));
            envSndPrfCntModel->item(i,1)->setForeground(QBrush(QColor(0,225,0)));
        }
    }
    else {
        pserVer->clear();
        envSndPrfCntModel->removeRows(0,envSndPrfCntModel->rowCount());
    }
}
/**
*   @project         FrameDemo
*   @brief           返回声剖数据来源标识
*   0 模型输入  1 人工输入  其他不限制
*   @author          XuYuanbing
*   @data            2018-08-03
*/
int groupSoundPro::getSoundProFrom()
{
    return ui->pcmDataFrom->getComboIndex();
}
/**
*   @project         FrameDemo
*   @brief           返回声剖类型标识
*   @author          XuYuanbing
*   @data            2018-08-03
*/
int groupSoundPro::getSoundProType()
{
    return ui->pcmSndProType->getComboIndex();
}
/**
*   @project         FrameDemo
*   @brief           返回声剖层数
*   @author          XuYuanbing
*   @data            2018-08-03
*/
int groupSoundPro::getSoundProNum()
{
    return ui->pcmSndProNum->getComboIndex();
}
/**
*   @project         FrameDemo
*   @brief           返回声剖数据
*   @author          XuYuanbing
*   @data            2018-08-03
*/
QVector<groupSoundPro::SOUNDPRO_DATA_S> groupSoundPro::getSoundProData()
{
    SOUNDPRO_DATA_S soundProData;
    QVector<SOUNDPRO_DATA_S> allSoundProData;
    for(int i = 0; i < usEffectiveSoundProNum;i++)
    {
        soundProData.ucId    = i + 1;
        soundProData.ucDepth = envSndPrfCntModel->item(i,0)->text().toShort();
        soundProData.ucSpeed = envSndPrfCntModel->item(i,1)->text().toShort();
        allSoundProData.push_back(soundProData);
    }
    return allSoundProData;
}

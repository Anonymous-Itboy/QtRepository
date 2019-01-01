/**
*   @project         FrameDemo
*   @brief           udp通信
*   @author          XuYuanbing
*   @data            2018-08-03
*/
#include <QSettings>
#include <QCoreApplication>
#include <QNetworkInterface>
#include <QObject>

#include "session_udp.h"

sessionUdp::sessionUdp():QObject(),oUdpSocket(0),irecvBufSize(1024)
{

}
bool sessionUdp::initSessionUdp(const QString configFilePath, int recvBufSize)
{
    udp_state = true;
    if(configFilePath.isEmpty())
    {
        qDebug() << "udp configFilePath is empty.";
        udp_state = false;
        return udp_state;
    }
    if(recvBufSize > irecvBufSize)
    {
        irecvBufSize = recvBufSize;
    }

    configFileInfo = QFileInfo(configFilePath);
    QSettings *configIniRead = new QSettings(configFilePath,QSettings::IniFormat);

    oRmtAddr  = QHostAddress(configIniRead->value("LocalAddress/multicast").toString());
    usRmtPort = configIniRead->value("ReceivingAddress/port").toInt();
    oLocAddr  = QHostAddress(configIniRead->value("LocalAddress/ip").toString());
    usLocPort = configIniRead->value("LocalAddress/port").toInt();

    qDebug() <<"LocalAddress/multicast:"<< oRmtAddr
             <<"LocalAddress/ip:"<< oLocAddr
             <<"LocalAddress/port:"<< usLocPort
             <<"ReceivingAddress/multicast:"<< configIniRead->value("ReceivingAddress/multicast").toString()
             <<"ReceivingAddress/port:"<< usRmtPort;

    if(oUdpSocket.bind(oLocAddr, usLocPort))
    {
        qDebug()<<"udp socket bind succeed.";
    }else {
        qDebug()<<"udp socket bind failed.";
        udp_state = false;
        return udp_state;
    }

/*//    QNetworkInterface localeNetwork;
//    QList<QNetworkInterface>  NetworkInterface = QNetworkInterface::allInterfaces();

//    for(QList<QNetworkInterface>::const_iterator it = NetworkInterface.constBegin();it!= NetworkInterface.constEnd();++it)
//    {
//        if((*it).humanReadableName() == QString::fromLocal8Bit("以太网"))
//        {
//            localeNetwork = QNetworkInterface::interfaceFromName((*it).name());
//            qDebug()<<localeNetwork.name();
//            qDebug()<<localeNetwork.humanReadableName();
//            qDebug()<<localeNetwork.hardwareAddress();
//            qDebug()<<"--------++++++++++++++++------";
//        }
//    }
//    bRet = oUdpSocket.joinMulticastGroup(QHostAddress(configIniRead->value("ReceivingAddress/multicast").toString()),localeNetwork);  //加入组播组*/

    if(oUdpSocket.joinMulticastGroup(QHostAddress(configIniRead->value("ReceivingAddress/multicast").toString())))
    {
        qDebug()<<"udp socket joinMulticastGroup succeed.";
    }
    else
    {
        qDebug()<<"udp socket joinMulticastGroup failed.";
        udp_state = false;
        return udp_state;
    }

    if(udp_state)
    {
        aucRecvBuf = new char[irecvBufSize];                                //接收缓存区
        connect(&oUdpSocket, SIGNAL(readyRead()), this, SLOT(onRecv()));    //连接接收信号槽
        qDebug()<<"udp init succeed.recvBufSize:"<< irecvBufSize;
    }else {
        qDebug()<<"udp init failed.";
    }
    return udp_state;
}

/**
*   @project         FrameDemo
*   @brief           数据接收
*   @author          XuYuanbing
*   @data            2018-08-03
*/
void sessionUdp::onRecv()
{
    qint64 llRet = oUdpSocket.readDatagram(aucRecvBuf,
                                           irecvBufSize,
                                           nullptr, nullptr);
    if(llRet <= 0)
    {
        qDebug() << "recv data failed.data size:" << llRet;
        return;
    }
    char *recvData =(char *) malloc(llRet);
    memset(recvData,0,llRet);
    memcpy(recvData,aucRecvBuf,llRet);
    recvQueue.push_back(*recvData);
    free(recvData);
    return;
}
/**
*   @project         FrameDemo
*   @brief           数据发送
*   @author          XuYuanbing
*   @data            2018-08-03
*/
void sessionUdp::onSend(char* cDatagram,int iDataLeng)
{
    if(iDataLeng <= 0)
    {
        qDebug()<<"send failed. send data leng:"<< iDataLeng;
        return;
    }
    int isendDataleng = iDataLeng;
    char *pcDatagram = new char[isendDataleng];  //发送缓存区
    if (nullptr == pcDatagram)
    {
        return;
    }
    memset(pcDatagram,0,isendDataleng);
    memcpy(pcDatagram,cDatagram,isendDataleng);
    qint64 llRet = oUdpSocket.writeDatagram(pcDatagram,
                                            isendDataleng,
                                            oRmtAddr,
                                            usRmtPort);
    if(llRet == -1)
    {
        qDebug("send failed. error:%d", oUdpSocket.error());
        return;
    }
    qDebug("sent datagram to %s recvPort:%d byte num:%d localAddr:%s localPort:%d",
           oRmtAddr.toString().toStdString().c_str(),
           usRmtPort,
           static_cast<int>(llRet),
           oLocAddr.toString().toStdString().c_str(),
           usLocPort
           );
    delete pcDatagram;
    return;
}

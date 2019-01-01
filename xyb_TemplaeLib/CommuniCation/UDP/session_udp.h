/**
*   @project         FrameDemo
*   @brief           udp通信协议
*   @author          XuYuanbing
*   @data            2018-08-03
*/
#include <QDebug>
#include <QFileInfo>
#include <QUdpSocket>

#ifndef SESSION_UDP_H
#define SESSION_UDP_H

#include <QUdpSocket>
#include <QVector>

class sessionUdp : public QObject
{
public:
    sessionUdp();
    bool initSessionUdp(const QString configFilePath,int recvBufSize);

public slots:
    void onRecv();
    void onSend(char *,int);

private:
    bool                udp_state;
    QFileInfo           configFileInfo;            //上传文件信息
    int                 irecvBufSize;              //接收缓冲区大小
    char*               aucRecvBuf;                //接收缓冲区
    QUdpSocket          oUdpSocket;                //udp套接字
    QHostAddress        oRmtAddr;                  //发送地址组播
    ushort              usRmtPort;                 //发送端口
    QHostAddress        oLocAddr;                  //本地地址
    ushort              usLocPort;                 //本地端口

    QVector<char> recvQueue;   //接收队列

};

#endif // SESSION_UDP_H

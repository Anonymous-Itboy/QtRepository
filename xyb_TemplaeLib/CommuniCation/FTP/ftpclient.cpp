#include "ftpclient.h"

ftpClient::ftpClient():
    ftp_state(false)
{

}

ftpClient::~ftpClient()
{
    accessManager->deleteLater();
}
bool ftpClient::initFtpClient(QString addr, int port, QString user, QString pass)
{
    QString meg;
    ftp_state = true;
    if(addr.isEmpty())
    {
        meg.append( "ftp server address is empty. ");
        ftp_state = false;
    }
    if(0 == port)
    {
        meg.append( "ftp server port is empty. ");
        ftp_state = false;
    }
    if(user.isEmpty())
    {
        meg.append("ftp server username is empty. ");
        ftp_state = false;
    }
    if(pass.isEmpty())
    {
        meg.append( "ftp server password is empty. ");
        ftp_state = false;
    }

    if(ftp_state)
    {
        ftp_server_address = addr;
        ftp_server_port    = port;
        ftp_login_username = user;
        ftp_login_password = pass;

        accessManager = new QNetworkAccessManager(this);
        accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
        progressBar = new QProgressBar();
        initConnect();

        qDebug()<<"ftp init succeed.";
    }else {
        qDebug()<<"ftp init failed."<<meg;
    }
    return ftp_state;
}
void ftpClient::initConnect()
{
    connect((QObject*)reply,SIGNAL(readyRead()),this,SLOT(readContent()));
    connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(loadProgress(qint64,qint64)));
    return;
}

/**
*   @project         FrameDemo
*   @brief           FTP上传
*   本地IP： 192.168.1.221
*           255.255.0.0
*   @author          XuYuanbing
*   @data            2018-11-22
*/
void ftpClient::uploadFile(const QString filePath)
{
    if(!ftp_state)
    {
        qDebug()<<"ftp not init.";
        return;
    }
    if(filePath.isEmpty())
    {
        qDebug()<<"ftp upload failed.filePath is empty.";
        return;
    }
    uploadFileInfo = QFileInfo(filePath);
    file = new QFile(filePath);
    file->open(QIODevice::ReadOnly);
    QByteArray byte_file = file->readAll();

    QUrl url("ftp://"+ftp_server_address+"/" + uploadFileInfo.fileName());
    url.setPort(ftp_server_port);
    url.setUserName(ftp_login_username);
    url.setPassword(ftp_login_password);

    QNetworkRequest request(url);
    reply = accessManager->put(request,byte_file);

    progressBar->setValue(0);
    progressBar->show();
    return;
}

void ftpClient::downloadFile(const QString saveName)
{
    if(!ftp_state)
    {
        qDebug()<<"ftp not init.";
        return;
    }
    if(saveName.isEmpty())
    {
        qDebug()<<"ftp download failed.saveName is empty.";
        return;
    }
    downloadFileName = saveName;
    file = new QFile(QApplication::applicationDirPath() + "/FTP" + downloadFileName);
    file->open(QIODevice::WriteOnly);

    QUrl url("ftp://"+ftp_server_address+"/" + downloadFileName);
    url.setPort(ftp_server_port);
    url.setUserName(ftp_login_username);
    url.setPassword(ftp_login_password);

    QNetworkRequest request(url);
    reply = accessManager->get(request);

    progressBar->setValue(0);
    progressBar->show();
    return;
}
/**
*   @project         FrameDemo
*   @brief           下载时向本地文件中写入数据
*   @author          XuYuanbing
*   @data            2018-11-22
*/
void ftpClient::readContent()
{
    file->write(reply->readAll());
    return;
}
/**
*   @project         FrameDemo
*   @brief           删除指针，更新和关闭文件
*   @author          XuYuanbing
*   @data            2018-11-22
*/
void ftpClient::replyFinished(QNetworkReply *)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        reply->deleteLater();
        file->flush();
        file->close();
    }
    else {
        qDebug()<<"ftp finished failed.";
    }
    return;
}
/**
*   @project         FrameDemo
*   @brief           更新进度条
*   @author          XuYuanbing
*   @data            2018-11-22
*/
void ftpClient::loadProgress(qint64 bytesSend, qint64 bytesTotal)
{
    qDebug()<<"ftp loaded: "<<bytesSend<<" of "<<bytesTotal;
    progressBar->setMaximum(bytesTotal);
    progressBar->setValue(bytesSend);
    return;
}
/**
*   @project         FrameDemo
*   @brief           错误输出
*   @author          XuYuanbing
*   @data            2018-11-22
*/
void ftpClient::loadError(QNetworkReply::NetworkError)
{
    qDebug()<<"ftp error:"<<reply->error();
    return;
}

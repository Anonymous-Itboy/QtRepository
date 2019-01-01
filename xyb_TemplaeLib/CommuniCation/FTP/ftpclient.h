#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QProgressBar>

class ftpClient : public QObject
{
    Q_OBJECT

public:
    explicit ftpClient();
    ~ftpClient();

    bool initFtpClient(QString addr,int port,QString user,QString pass);
    void uploadFile(const QString filePath);
    void downloadFile(const QString saveName);

private slots:

    void readContent();
    void replyFinished(QNetworkReply*);
    void loadError(QNetworkReply::NetworkError);
    void loadProgress(qint64 bytesSend,qint64 bytesTotal);

private:
    bool                   ftp_state;
    QString                ftp_server_address;
    int                    ftp_server_port;
    QString                ftp_login_username;
    QString                ftp_login_password;

    QNetworkAccessManager *accessManager;
    QNetworkReply *reply;
    QProgressBar  *progressBar;
    QFile         *file;
    QFileInfo      uploadFileInfo;                          //上传文件信息
    QString        downloadFileName;                        //下载文件名
    QString        downloadFileType;                        //下载文件类型

    void initConnect();
};

#endif // FTPCLIENT_H

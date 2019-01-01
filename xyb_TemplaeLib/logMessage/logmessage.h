#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

#include <QWidget>
#include <QTextBrowser>
#include <QLabel>

class logMessage : public QWidget
{
    Q_OBJECT
public:
    enum showWayFlag
    {
        windowShow = 0,
        outFile,
        All
    };

    explicit logMessage(QWidget *parent = nullptr,int outFlag = 0,int maxShowCount = 100);
    void messageOutput(QtMsgType type,
                       const QMessageLogContext &context,
                       const QString &msg);

signals:

private:
    QTextBrowser *logWindow;
    uint uiColorFlg;
    QLabel *countNum;
    bool showWindowFlag;
    bool outFileFlag;

};

#endif // LOGMESSAGE_H

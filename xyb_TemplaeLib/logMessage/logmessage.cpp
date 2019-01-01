/**
*   @project         FrameDemo
*   @brief           日志处理
*   @author          XuYuanbing
*   @data            2018-08-08
*/

#include <QDateTime>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMutex>

#include "logmessage.h"

logMessage::logMessage(QWidget *parent,
                       int windowShow,
                       int maxShowCount)
                     : QWidget(parent)
{
    switch (windowShow) {
    case logMessage::windowShow:
    {
        showWindowFlag = true;
        break;
    }
    case logMessage::outFile:
    {
        outFileFlag = true;
        break;
    }
    case logMessage::All:
    {
        showWindowFlag = true;
        outFileFlag = true;
        break;
    }
    default:
        showWindowFlag = true;
        outFileFlag = false;
        break;
    }
    logWindow  = new QTextBrowser(this);
    logWindow->document()->setMaximumBlockCount(maxShowCount);
    logWindow->setStyleSheet("border:none");
    countNum = new QLabel(this);
    uiColorFlg = 0;
    return;
}
/**
*   @project         FrameDemo
*   @brief           摘要
*   @author          XuYuanbing
*   @data            2018-08-08
*/
void logMessage::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
      switch(uiColorFlg)
      {
        case 0:
        {
            logWindow->setTextColor(QColor(0,200,0));
            uiColorFlg = 1;
            break;
        }
          case 1:
        {
            logWindow->setTextColor(QColor(200,200,0));
            uiColorFlg = 0;
            break;
        }
        default:
          break;
      }

      QString logMessage;
      QString data_time;
      //QByteArray localMsg = msg.toLocal8Bit();
      switch (type) {
      case QtDebugMsg:
          logMessage = QString(" Debug:");
          break;
      case QtInfoMsg:
          logMessage = QString(" Info:");
          break;
      case QtWarningMsg:
          logMessage = QString(" Warning:");
          break;
      case QtCriticalMsg:
          logMessage = QString(" Critical:");
          break;
      case QtFatalMsg:
          logMessage = QString(" Fatal:");
          abort();
      }

      if(showWindowFlag)
      {
          data_time = QDateTime::currentDateTime().toString("hh:mm:ss");
          logWindow->append(data_time.append(logMessage.append(msg)));
      }
      if(outFileFlag)
      {
          data_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
          QString context_info = QString("file:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
          QString current_data = QString("(%1)").arg(data_time);
          QString outMessage = QString("%1 %2 %3 %4").arg(context_info).arg(current_data).arg(logMessage).arg(msg);

          QFile file("log.txt");
          file.open(QIODevice::WriteOnly | QIODevice::Append);
          QTextStream text_stream(&file);
          text_stream << outMessage <<"\r\n";
          file.flush();
          file.close();
      }
      mutex.unlock();
      return;
}
/**
*   @project         FrameDemo
*   @brief           窗口显示
*   @author          XuYuanbing
*   @data            2018-08-08
*/


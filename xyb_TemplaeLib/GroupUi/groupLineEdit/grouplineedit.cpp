/**
*   @project         FrameDemo
*   @brief           组合输入框
*   @author          XuYuanbing
*   @data            2018-08-01
*/
#include "grouplineedit.h"

groupLineEdit::groupLineEdit(QWidget *parent) :
    QWidget(parent)
{
    unitIsShow = false;
    label    = new QLabel(this);
    label_2  = new QLabel(this);
    lineEdit = new QLineEdit(this);
    label_2->hide();
    return;
}

groupLineEdit::~groupLineEdit()
{
    delete label;
    delete label_2;
    delete lineEdit;
}
/**
*   @project         FrameDemo
*   @brief           设置标题
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupLineEdit::setTitle(const QString &strTitle)
{
    label->setText(strTitle);
    return;
}
/**
*   @project         FrameDemo
*   @brief           是否显示单位
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupLineEdit::showUnit(bool status,const QString &strUnit)
{
    unitIsShow = status;
    if(status)
    {
        label_2->show();
        label_2->setText(strUnit);
    }
    else {
        label_2->hide();
    }
    return;
}
/**
*   @project         FrameDemo
*   @brief           获取输入的值
*   @author          XuYuanbing
*   @data            2018-08-01
*/
QString& groupLineEdit::getValue()
{
    QString strTitle = lineEdit->text();
    return strTitle;
}
/**
*   @project         FrameDemo
*   @brief           设置输入值范围
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupLineEdit::setValidator(int iMin,int iMax)
{
    lineEdit->setValidator(new QIntValidator(iMin,iMax,this));
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置输入框提示信息
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupLineEdit::setPlaceholderText(const QString &strPlaceholderText)
{
    lineEdit->setPlaceholderText(strPlaceholderText);
    return;
}
/**
*   @project         FrameDemo
*   @brief           清空输入框
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupLineEdit::cleanInput()
{
    lineEdit->setText("");
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置输入框默认显示值
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupLineEdit::setInputText(QString text)
{
    lineEdit->setText(text);
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置组合框是否可编辑（状态）
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupLineEdit::setEditStatus(bool status)
{
    lineEdit->setEnabled(status);
    if(status)
    {
        label->setStyleSheet("color: rgb(220, 220, 220);background-color: rgba(60, 60, 60,0);");
        lineEdit->setStyleSheet("color: rgb(220, 220, 220);background-color: rgba(60, 60, 60,0);");
        if(unitIsShow)
        {
            label_2->setStyleSheet("color: rgb(220, 220, 220);background-color: rgba(60, 60, 60,0);");
        }
    }else {
         label->setStyleSheet("color: rgb(150, 150, 150);background-color: rgba(255, 255, 255, 0);");
         lineEdit->setStyleSheet("color: rgb(150, 150, 150);background-color: rgba(255, 255, 255, 0);");
         if(unitIsShow)
         {
             label_2->setStyleSheet("color: rgb(150, 150, 150);background-color: rgba(255, 255, 255, 0);");
         }
    }
    return;
}

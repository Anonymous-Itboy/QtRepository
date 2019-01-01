#include "groupcombobox.h"

groupComboBox::groupComboBox(QWidget *parent) :
    QWidget(parent)
{
    comboBox = new QComboBox(this);
    lable    = new QLabel(this);
    QHBoxLayout *playout = new QHBoxLayout();
    playout->addWidget(lable);
    playout->addWidget(comboBox);
    setLayout(playout);
    connect(comboBox, SIGNAL(currentIndexChanged(int)),
                this, SIGNAL(currentIndexChanged(int)));
}

groupComboBox::~groupComboBox()
{

}
/**
*   @project         FrameDemo
*   @brief           设置下拉框的标题
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupComboBox::setTitle(const QString &strTitle)
{
    lable->setText(strTitle);
    return;
}
/**
*   @project         FrameDemo
*   @brief           添加下拉框选项(多个）
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupComboBox::addItems(const QList<QString> itemList)
{
    for(int i = 0;i < itemList.size();i++)
    {
        comboBox->addItem(itemList[i]);
    }
    return;
}
/**
*   @project         FrameDemo
*   @brief           添加下拉框选项(单个）
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupComboBox::addItem(const QString &strItem)
{
    comboBox->addItem(strItem);
    return;
}
/**
*   @project         FrameDemo
*   @brief           获取选中的下标
*   @author          XuYuanbing
*   @data            2018-08-01
*/
int groupComboBox::getComboIndex()
{
    return comboBox->currentIndex();
}
/**
*   @project         FrameDemo
*   @brief           设置是否可用（状态）
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupComboBox::setStatusEdit(bool status)
{
    comboBox->setEnabled(status);
    if(status)
    {
        lable->setStyleSheet("");
        comboBox->setStyleSheet("");
    }
    else {
        lable->setStyleSheet("color: rgb(150, 150, 150);background-color: rgba(255, 255, 255, 0);");
        comboBox->setStyleSheet("color: rgb(150, 150, 150);");
    }
    return;
}
/**
*   @project         FrameDemo
*   @brief           设置默认选中子项
*   @author          XuYuanbing
*   @data            2018-08-01
*/
void groupComboBox::setCurrentIndex(int index)
{
    comboBox->setCurrentIndex(index);
    return;
}
/**
*   @project         FrameDemo
*   @brief           返回标题指针
*   @author          XuYuanbing
*   @data            2018-08-01
*/
QLabel* groupComboBox::returnLabel()
{
    return lable;
}
/**
*   @project         FrameDemo
*   @brief           返回下拉框指针
*   @author          XuYuanbing
*   @data            2018-08-01
*/
QComboBox* groupComboBox::returnComboBox()
{
    return comboBox;
}

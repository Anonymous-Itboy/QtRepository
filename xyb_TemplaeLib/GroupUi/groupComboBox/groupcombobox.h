/**
*   @name            %{Current:Name}
*   @brief           组合下拉框
*   @author          XuYuanbing
*   @data            2018-07-31
*/
#ifndef GROUPCOMBOBOX_H
#define GROUPCOMBOBOX_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QList>
#include <QHBoxLayout>

class groupComboBox : public QWidget
{
    Q_OBJECT

public:
    explicit groupComboBox(QWidget *parent = 0);
    ~groupComboBox();

    void setTitle(const QString &strTitle);
    void addItems(const QList<QString> itemList);
    void addItem(const QString &strItem);
    int  getComboIndex();
    void setStatusEdit(bool);
    void setCurrentIndex(int index);

    QLabel* returnLabel();
    QComboBox* returnComboBox();

private:
    QComboBox *comboBox;
    QLabel*    lable;

signals:
    void currentIndexChanged(int index);
};

#endif // GROUPCOMBOBOX_H

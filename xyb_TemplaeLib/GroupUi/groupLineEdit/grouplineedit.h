#ifndef GROUPLINEEDIT_H
#define GROUPLINEEDIT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>

class groupLineEdit : public QWidget
{
    Q_OBJECT

public:
    explicit groupLineEdit(QWidget *parent = 0);
    ~groupLineEdit();

    void setTitle(const QString &strTitle);
    void showUnit(bool,const  QString &strUnit);
    QString& getValue();
    void setValidator(int,int);
    void setPlaceholderText(const QString &strPlaceholderText);
    void cleanInput();
    void setInputText(const QString text);
    void setEditStatus(bool);

    QLabel* returnTitle();
    QLabel* returnUnit();
    QLineEdit* returnLineEdit();

private:
    QLabel*      label;
    QLabel*      label_2;
    QLineEdit*   lineEdit;
    bool         unitIsShow;
};

#endif // GROUPLINEEDIT_H

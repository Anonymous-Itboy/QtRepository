#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

//test
#include "GroupUi/groupComboBox/groupcombobox.h"
#include "dataBase/dataBase.h"
#include "GroupUi/groupPolarChart/grouppolarchart.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    groupPolarChart *h1;
    QTimer *timer;

private slots:
    void updataPos();
};

#endif // MAINWINDOW_H

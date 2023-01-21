#ifndef CHOOSEREMINDDATETIME_H
#define CHOOSEREMINDDATETIME_H
#include <QMainWindow>
#include <QWidget>
#include "mainwindow.h"
class MainWindow;
using namespace std;
namespace Ui {
class chooseRemindDateTime;
}

class chooseRemindDateTime : public QWidget
{
    Q_OBJECT

public:
    explicit chooseRemindDateTime(QWidget *parent = 0,MainWindow *window=0);
    ~chooseRemindDateTime();

public slots:
    void enableButton();
    void disableButton();
    void getDateTime();
    void removeRemindTime();

signals:
    void closeWindow();

private:
    Ui::chooseRemindDateTime *ui;
    MainWindow *main;
};

#endif // CHOOSEREMINDDATETIME_H

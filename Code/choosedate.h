#ifndef CHOOSEDATE_H
#define CHOOSEDATE_H
#include <QWidget>
#include "mainwindow.h"
#include <QMainWindow>
class MainWindow;
using namespace std;
namespace Ui {
class ChooseDate;
}

class ChooseDate : public QWidget
{
    Q_OBJECT

public:

    explicit ChooseDate(QWidget *parent = 0,MainWindow *window=0 );
    ~ChooseDate();
    void enableButton();
    void disableButton();
    void enableRemindButton();
    void disableRemindButton();
    void getDate();
    void getRemindTime();
signals:
    void closeWindow();
private:
    Ui::ChooseDate *ui;
    MainWindow *main;
};

#endif // CHOOSEDATE_H

#ifndef CHOOSEDUEDATE_H
#define CHOOSEDUEDATE_H
#include <QMainWindow>
#include <QWidget>
#include "mainwindow.h"
class MainWindow;
using namespace std;

namespace Ui {
class ChooseDueDate;
}

class ChooseDueDate : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseDueDate(QWidget *parent = 0,MainWindow *window=0);
    ~ChooseDueDate();
public slots:
    void enableButton();
    void disableButton();
    void getDate();
    void removeDueDate();
signals:
    void closeWindow();

private:
    Ui::ChooseDueDate *ui;
    MainWindow *main;
};

#endif // CHOOSEDUEDATE_H

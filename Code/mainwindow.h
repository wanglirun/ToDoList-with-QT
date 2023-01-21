#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QMainWindow>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include "choosedate.h"
#include "task.h"
#include <QDialog>
#include <QInputDialog>
#include <QDir>
#include <QDateTimeEdit>
#include "choosereminddatetime.h"
#include "chooseduedate.h"
#include "QMap"
#include <QListWidget>
#include <QListWidgetItem>
#include "group.h"
#include <QVector>
class Group;
class Task;
class ChooseDate;
class chooseRemindDateTime;
class ChooseDueDate;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int curGroupId;
    Task* task;
    Group* group;
    ChooseDate* window;
    chooseRemindDateTime* chooseRemindDateTimeWindow;
    ChooseDueDate* chooseDueDateWindow;
    QSqlDatabase database;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initDB();
public slots:
    void onAddTaskButtonClicked();
    void inputTaskTitle();
    void taskDone(QTreeWidgetItem*item);
    void onChooseDataClicked();
    void onRefreshButtonClicked();
    void setPriority(int index);
    void setRemindTime();
    void setDueTime();
    void setDescription();
    void setNote();
    void onMyTaskPageButtonClicked();
    void onMyFrontPageButtonClicked();
    void addGroupOrChangePage(QListWidgetItem* item);
    void switchGroup(QListWidgetItem* item);
    void refresh();
    void sort(int index);
    void setDone();
    void setTitle();
    void modifyTitle();
public:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

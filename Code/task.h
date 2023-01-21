#ifndef TASK_H
#define TASK_H
#include<QString>
#include<QObject>
#include "mainwindow.h"
#include <QDateTime>
#include <QTreeWidgetItem>
class MainWindow;
class Task:public QObject{
    Q_OBJECT
public:
    int m_id;
    int m_priority;
    int m_isDone;
    QString m_title;
    QString m_description;
    QString m_note;
    QString m_createTime;//"yyyy-MM-dd hh:mm:ss"
    QString m_remindTime;//"yyyy-MM-dd hh:mm"
    QString m_dueTime;//"yyyy-MM-dd"
    QTreeWidgetItem* m_item;
public slots:
    //void inputTaskTitle();

public:
    Task(MainWindow* main);
    int insertTask();//返回id
    void clear();
    void setRemindTime();
    void setDueTime();
    void setDescription();
    void setNote();
    void setPriority();
    void setDone();
    void setUndone();
    void setTitle();
private:
    MainWindow* main;
};
#endif // TASK_H

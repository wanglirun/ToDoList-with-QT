#ifndef GROUP_H
#define GROUP_H
#include<QString>
#include<QObject>
#include "mainwindow.h"
#include <QDateTime>
#include <QTreeWidgetItem>
class MainWindow;
class Group:public QObject{
    Q_OBJECT
public:
    explicit Group(MainWindow* main);
    void clear();
    int addGroup();
    bool canBeAdded(QString);
    QString getGroupNameById();
    int m_groupId;
    QString m_groupName;

private:
    MainWindow* main;
};

#endif // GROUP_H

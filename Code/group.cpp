#include "group.h"

Group::Group(MainWindow* main)
{
    this->main=main;
    m_groupId=0;
    m_groupName.clear();
}

void Group::clear()
{
    m_groupId=0;
    m_groupName.clear();
}
int Group::addGroup()
{
    bool ok=main->database.open();
    if(ok)
        qDebug()<<"打开数据库成功！";
    else qDebug()<<"打开数据库失败！";
    //插入mygroups
    QSqlQuery query=QSqlQuery(main->database);
    query.exec("select * from mygroups");
    while(query.next())
    {
        qDebug()<<query.value(1).toString();
    }
    query.prepare("insert into mygroups values(?,?)");//千万别写错query,注意value加s
    query.bindValue(1,m_groupName);
    ok=query.exec();
    if(ok)
        qDebug()<<"插入组别成功！";
    else
        qDebug()<<query.lastError().text();
    m_groupId=query.lastInsertId().toInt();
    qDebug()<<m_groupId;
    //向数据库中插入新表
    QString str=QString("create table '%1'(id integer primary key autoincrement,"
                        "title text,"
                        "createTime text,"
                        "dueTime text,"
                        "remindTime text,"
                        "description text,"
                        "note text,"
                        "priority int,"
                        "isDone int)").arg(m_groupName);
    if(!query.exec(str)){
        qDebug() << query.executedQuery();
        qDebug() << query.lastError();
    }
    else qDebug()<<"创建新表成功！";
    main->database.close();
    return m_groupId;
}
//记得不能有重复的表
bool Group::canBeAdded(QString str)
{
    bool ok=main->database.open();
    if(ok)
        qDebug()<<"打开数据库成功！";
    else qDebug()<<"打开数据库失败！";
    //插入mygroups
    QSqlQuery query=QSqlQuery(main->database);
    query.exec("select * from mygroups");
    while(query.next())
    {
        if(query.value(1).toString()==str)
            return false;
    }
    return true;
}

QString Group::getGroupNameById()
{
    bool ok=main->database.open();
    if(!ok)
        qDebug()<<"获取组名时，打开数据库失败！";
    QSqlQuery query=QSqlQuery(main->database);
    ok=query.exec(QString("select * from mygroups where id=%1").arg(main->curGroupId));
    if(!ok) qDebug()<<main->database.lastError();
    QString str;
    while(query.next())
       str=query.value(1).toString();
    return str;
}

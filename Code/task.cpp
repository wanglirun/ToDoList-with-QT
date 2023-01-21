#include"task.h"
void Task::clear()
{
    m_item=nullptr;
    m_id=0;
    m_priority=0;
    m_isDone=0;
    m_title.clear();
    m_description.clear();
    m_note.clear();
    m_createTime.clear();//"yyyy-MM-dd hh:mm:ss"
    m_remindTime.clear();//"yyyy-MM-dd hh:mm"
    m_dueTime.clear();//"yyyy-MM-dd"
}
Task::Task(MainWindow *main)
{
    this->main=main;
}
int Task::insertTask()
{
    bool ok=main->database.open();
    if(ok)
        qDebug()<<"打开数据库成功！";
    else qDebug()<<"打开数据库失败！";
    m_createTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//存现在的时间
    QSqlQuery query=QSqlQuery(main->database);

    QString str=QString("insert into '%1' values(?,?,?,?,?,?,?,?,?)").arg(main->group->getGroupNameById());
    qDebug()<<str;
    query.prepare(str);//千万别写错query,注意value加s
    query.bindValue(1,m_title);
    query.bindValue(2,m_createTime);
    query.bindValue(3,m_dueTime);
    query.bindValue(4,m_remindTime);
    query.bindValue(5,m_description);
    query.bindValue(6,m_note);
    query.bindValue(7,m_priority);
    query.bindValue(8,0);
    ok=query.exec();
    if(ok){
        qDebug()<<"插入task成功！";
    }
    else
        qDebug()<<query.lastError().text();
    int id=query.lastInsertId().toInt();
    qDebug()<<m_createTime;
    main->database.close();
    return id;
}
void Task::setTitle()
{
    main->database.open();
    QSqlQuery query;
    QString tableName=main->group->getGroupNameById();
    QString str=QString("select * from '%1' where id = ?").arg(tableName);
    query.prepare(str);
    query.bindValue(0,m_id);
    query.exec();
    while(query.next()){
        if(!query.exec(QString(R"(UPDATE '%3' SET title='%2' WHERE id=%1;)")
                       .arg(m_id).arg(m_title).arg(tableName)))
            qDebug()<<main->database.lastError();
        else qDebug()<<"title更新成功！";
    }
    main->refresh();
    main->onRefreshButtonClicked();
    main->database.close();
}
void Task::setDescription()
{
    main->database.open();
    QSqlQuery query;
    QString tableName=main->group->getGroupNameById();
    QString str=QString("select * from '%1' where id = ?").arg(tableName);
    query.prepare(str);
    query.bindValue(0,m_id);
    query.exec();
    while(query.next()){
        if(!query.exec(QString(R"(UPDATE '%3' SET description='%2' WHERE id=%1;)")
                       .arg(m_id).arg(m_description).arg(tableName)))
            qDebug()<<main->database.lastError();
        else qDebug()<<"描述更新成功！";
    }
    main->database.close();

}
void Task::setNote()
{
    main->database.open();
    QSqlQuery query;
    QString tableName=main->group->getGroupNameById();
    QString str=QString("select * from '%1' where id = ?").arg(tableName);
    query.prepare(str);
    query.bindValue(0,m_id);
    query.exec();
    while(query.next()){
        if(!query.exec(QString(R"(UPDATE '%3' SET note='%2' WHERE id=%1;)")
                       .arg(m_id).arg(m_note).arg(tableName)))
            qDebug()<<main->database.lastError();
        else qDebug()<<"备注更新成功！";
    }
    main->database.close();

}
void Task::setPriority()
{
    main->database.open();
    QSqlQuery query;
    QString tableName=main->group->getGroupNameById();
    QString str=QString("select * from '%1' where id = ?").arg(tableName);
    query.prepare(str);
    query.bindValue(0,m_id);
    query.exec();
    while(query.next()){
        if(!query.exec(QString(R"(UPDATE '%3' SET priority=%2 WHERE id=%1;)")
                       .arg(m_id).arg(m_priority).arg(tableName)))
            qDebug()<<main->database.lastError();
        else qDebug()<<"优先级更新成功！";
    }
    main->database.close();
}
void Task::setRemindTime()
{
    main->database.open();
    QSqlQuery query;
    QString tableName=main->group->getGroupNameById();
    QString str=QString("select * from '%1' where id = ?").arg(tableName);
    query.prepare(str);
    query.bindValue(0,m_id);
    query.exec();
    while(query.next()){
        if(!query.exec(QString(R"(UPDATE '%3' SET remindTime='%2' WHERE id=%1;)")
                       .arg(m_id).arg(m_remindTime).arg(tableName)))
            qDebug()<<main->database.lastError();
        else qDebug()<<"提醒时间更新成功！"<<m_remindTime;
    }
    main->database.close();
}
void Task::setDueTime()
{
    main->database.open();
    QSqlQuery query;
    QString tableName=main->group->getGroupNameById();
    QString str=QString("select * from '%1' where id = ?").arg(tableName);
    query.prepare(str);
    query.bindValue(0,m_id);
    query.exec();
    while(query.next()){
        if(!query.exec(QString(R"(UPDATE '%3' SET dueTime='%2' WHERE id=%1;)")
                       .arg(m_id).arg(m_dueTime).arg(tableName)))
            qDebug()<<main->database.lastError();
        else qDebug()<<"截止时间更新成功！"<<m_dueTime;
    }
    main->database.close();
}
void Task::setDone()
{
    QString tableName=main->group->getGroupNameById();
    QString str=QString("select * from '%1' where id = ?").arg(tableName);
    main->database.open();
    QSqlQuery query;
    query.prepare(str);
    query.bindValue(0,m_id);
    query.exec();
    while(query.next()){
        if(!query.exec(QString(R"(UPDATE '%3' SET isDone=%2 WHERE id=%1;)")
                       .arg(m_id).arg(1).arg(tableName)))
            qDebug()<<main->database.lastError();
        else qDebug()<<"已完成该任务！";
    }
    main->database.close();

}
void Task::setUndone()
{
    QString tableName=main->group->getGroupNameById();
    QString str=QString("select * from '%1' where id = ?").arg(tableName);
    main->database.open();
    QSqlQuery query;
    query.prepare(str);
    query.bindValue(0,m_id);
    query.exec();
    while(query.next()){
        if(!query.exec(QString(R"(UPDATE '%3' SET isDone=%2 WHERE id=%1;)")
                       .arg(m_id).arg(0).arg(tableName)))
            qDebug()<<main->database.lastError();
    }
    main->database.close();
}

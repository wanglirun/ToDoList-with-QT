#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->AddTask->setCurrentIndex(0);
    ui->taskSetting->setCurrentIndex(0);
    ui->taskList->setColumnWidth(0,200);
    ui->taskList->setColumnWidth(1,100);
    ui->taskList->setColumnWidth(2,170);
    ui->taskList->setColumnWidth(3,180);
    ui->taskList_2->setColumnWidth(0,200);
    ui->taskList_2->setColumnWidth(1,100);
    ui->taskList_2->setColumnWidth(2,170);
    ui->taskList_2->setColumnWidth(3,180);

    ui->sortBox->setVisible(false);
    ui->groupRefreshButton->setVisible(false);
    ui->groupList->item(0)->setData(Qt::UserRole,0);
    ui->stackTaskList->setCurrentIndex(0);
    initDB();//初始化数据库
    curGroupId=1;
    task=new Task(this);
    group=new Group(this);
    window=nullptr;
    chooseDueDateWindow=nullptr;
    chooseDueDateWindow=nullptr;
    //绑定槽函数和信号
    void(QTreeWidget::*itemClicked)(QTreeWidgetItem*,int)=&QTreeWidget::itemClicked;
    void(MainWindow::*taskDone)(QTreeWidgetItem*)=&MainWindow::taskDone;
    connect(ui->taskList,itemClicked,this,taskDone);//若任务完成，则删除该任务
    connect(ui->clickToAddTask,&QPushButton::clicked,this,
            &MainWindow::onAddTaskButtonClicked);//跳转至输入task的stack页面
    connect(ui->InputToAddTask,&QLineEdit::returnPressed,this,&MainWindow::inputTaskTitle);//回车添加task
    connect(ui->chooseDate,&QPushButton::clicked,this,&MainWindow::onChooseDataClicked);//弹出设置时间窗口
    connect(ui->refreshButton,&QPushButton::clicked,this,&MainWindow::onRefreshButtonClicked);//刷新
    //task的设置面板
    void(QComboBox::*activated)(int)=&QComboBox::activated;
    void(MainWindow::*setPriority)(int)=&MainWindow::setPriority;
    connect(ui->prioritySet,activated,this,setPriority);
    connect(ui->modifyTaskTitleButton,&QPushButton::clicked,this,&MainWindow::setTitle);
    connect(ui->modifyTaskTitle,&QLineEdit::returnPressed,this,&MainWindow::modifyTitle);
    connect(ui->setDuetime,&QPushButton::clicked,this,&MainWindow::setDueTime);
    connect(ui->setRemindtime,&QPushButton::clicked,this,&MainWindow::setRemindTime);
    connect(ui->setDescription,&QPushButton::clicked,this,&MainWindow::setDescription);
    connect(ui->setNote,&QPushButton::clicked,this,&MainWindow::setNote);
    connect(ui->taskTitle,&QCheckBox::toggled,this,&setDone);
    //PAGE：我的待办
    connect(ui->MyTaskPageButton,&QPushButton::clicked,this,&MainWindow::onMyTaskPageButtonClicked);
    connect(ui->MyFrontPageButton,&QPushButton::clicked,this,&MainWindow::onMyFrontPageButtonClicked);
    connect(ui->groupList,SIGNAL(itemClicked(QListWidgetItem*)),this,
            SLOT(addGroupOrChangePage(QListWidgetItem*)));
    connect(ui->groupRefreshButton,&QPushButton::clicked,this,&MainWindow::refresh);
    connect(ui->taskList_2,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(taskDone(QTreeWidgetItem*)));

    void(MainWindow::*sort)(int)=&MainWindow::sort;
    connect(ui->sortBox,activated,this,sort);


}

MainWindow::~MainWindow()
{
    delete ui;
}

/*****************************************我的首页************************************/
//初始化数据库，创建<我的待办>表,<我的完成>表，<我的分组>表
void MainWindow::initDB()
{
    database = QSqlDatabase::addDatabase("QSQLITE");

    database.setDatabaseName("Todolist.sqlite3");
    if(!database.open())
        qDebug() << "Error: Failed to connect database." << database.lastError();
    else
        qDebug() << "Succeed to connect database.";
    QSqlQuery query(database);
    //清空数据库
    if(query.exec("select name from sqlite_master where type='table'"))
        qDebug()<<"选择所有表成功!";
    QVector<QString> tableNames;
    while(query.next()){
        tableNames.push_back(query.value("name").toString());
        qDebug()<<query.value("name").toString();
    }
    for(int i=0;i<tableNames.size();++i){
        QString str=QString("drop table '%1'").arg(tableNames[i]);
        bool ok=query.exec(str);
        if(!ok) {
            qDebug() << query.executedQuery();
            qDebug() << query.lastError();
        }
        else qDebug()<<"删除"<<str;
    }
    //query = database.exec("DROP TABLE mytasks");//先清空表，记得删掉
    //query = database.exec("DROP TABLE mygroups");

    bool ok=query.exec("create table mytasks(id integer primary key autoincrement,"
                       "title text,"
                       "createTime text,"
                       "dueTime text,"
                       "remindTime text,"
                       "description text,"
                       "note text,"
                       "priority int,"
                       "isDone int)");
    if(!ok)
        qDebug()<<query.lastError().text();
    ok=query.exec("create table mygroups(id integer primary key autoincrement,"
                  "name text)");//
    if(!ok)
        qDebug()<<query.lastError().text();
    query.prepare("insert into mygroups values(?,?)");
    query.bindValue(1,"mytasks");
    ok=query.exec();
    if(!ok)
        qDebug()<<query.lastError().text();
    database.close();

}
//点击新增task的按钮
void MainWindow::onAddTaskButtonClicked()
{
    ui->AddTask->setCurrentIndex(1);
    task->clear();
    ui->taskSetting->setCurrentIndex(0);
}
//按回车，新增task。
void MainWindow::inputTaskTitle()
{
    QString str=ui->InputToAddTask->text();
    if(!str.isEmpty()){
        task->m_title=str;
        int id=task->insertTask();
        //qDebug()<<task->m_dueTime;
        ui->InputToAddTask->clear();
        QTreeWidgetItem *item;
        if(curGroupId==1)
            item = new QTreeWidgetItem(ui->taskList->topLevelItem(0));
        else
            item = new QTreeWidgetItem(ui->taskList_2->topLevelItem(0));
        item->setText(0,task->m_title);
        item->setCheckState(0,Qt::Unchecked);
        //qDebug()<<task->m_createTime;
        item->setText(3,task->m_createTime);
        item->setData(0,Qt::UserRole,id);//设置item的id，方便后续索引
    }
    ui->AddTask->setCurrentIndex(0);
    onRefreshButtonClicked();
    refresh();

}

//刷新
void MainWindow::onRefreshButtonClicked()
{
    for(int i=0;i<4;++i)
        ui->taskList->topLevelItem(i)->takeChildren();//删除顶层节点的所有子节点
    database.open();
    QSqlQuery query;
    int level=0;
    query.exec("select * from mytasks");
    while(query.next()){
        if(query.value(8).toInt()==0){
            QString str=query.value(3).toString();
            if(!str.isEmpty()){
                //qDebug()<<str;
                QDate date=QDate::fromString(str,"yyyy-MM-dd");
                QDate curDate=QDate::currentDate();
                if(date<curDate)
                    level=3;
                else{
                    QDate afterSevenDate=curDate.addDays(7);
                    if(date>=afterSevenDate)
                        level=2;
                    else level=1;
                }
            }

            QTreeWidgetItem *item = new QTreeWidgetItem(ui->taskList->topLevelItem(level));
            item->setText(0,query.value(1).toString());
            item->setCheckState(0,Qt::Unchecked);
            item->setData(0,Qt::UserRole,query.value(0).toInt());
            QString temp;
            if(!(temp=query.value(3).toString()).isEmpty())
                item->setText(1,temp);
            if(!(temp=query.value(4).toString()).isEmpty()){
                item->setText(2,temp);
                item->setIcon(2,QIcon(":/icons/icon/setRemindTime.png"));
             }
            if(!(temp=query.value(2).toString()).isEmpty())
              {  //qDebug()<<temp;
                item->setText(3,temp);}
            if(!(temp=query.value(5).toString()).isEmpty())
            {
                item->setText(4,temp);
            }
            if(!(temp=query.value(6).toString()).isEmpty())
            {
                item->setText(5,temp);
            }

            switch(query.value(7).toInt()){
                case 1:
                    item->setIcon(0,QIcon(":/icons/icon/lowPriority.png"));
                    break;
                case 2:
                    item->setIcon(0,QIcon(":/icons/icon/midPriority.png"));
                    break;
                case 3:
                    item->setIcon(0,QIcon(":/icons/icon/highPriority.png"));
                    break;
            }
       }
        level=0;
    }

    database.close();
    for(int i=0;i<4;++i)
        ui->taskList->topLevelItem(i)->setExpanded(1);
}

//当task被选中/点击task
void MainWindow::taskDone(QTreeWidgetItem*item)
{

    if(item->checkState(0)==Qt::Checked){//当选中时，将isDone置1
        //信号和槽如何传递参数
        int id=item->data(0,Qt::UserRole).toInt();
        item->parent()->removeChild(item);
        task->m_id=id;
        task->setDone();
        refresh();
        //onRefreshButtonClicked();
    }
    /*else if(item->checkState(0)==Qt::Unchecked){
        int id=item->data(0,Qt::UserRole).toInt();
        task->m_id=id;
        task->setUndone();
        //refresh();
    }*/

    //qDebug()<<item->parent();
    if(item->parent()!=NULL){//当task被点击时，出现设置面板
        ui->taskSetting->setCurrentIndex(1);
        ui->stackEditTitle->setCurrentIndex(1);
        ui->taskTitle->setText(item->text(0));
        ui->taskTitle->setChecked(false);
        task->clear();
        task->m_id=item->data(0,Qt::UserRole).toInt();
        task->m_item=item;
    }

}
//选择时间
void MainWindow::onChooseDataClicked()
{
    window=new ChooseDate(0,this);
    window->setAttribute(Qt::WA_ShowModal,true);
    window->setAttribute(Qt::WA_DeleteOnClose,true);
    window->show();
}
/****************************************任务设置面板***********************************/
//设置（改变task名字）
void MainWindow::setTitle()
{
    ui->stackEditTitle->setCurrentIndex(0);

}
void MainWindow::modifyTitle()
{
    task->m_title=ui->modifyTaskTitle->text();
    qDebug()<<task->m_title;
    if(!task->m_title.isEmpty())
        task->setTitle();
    ui->stackEditTitle->setCurrentIndex(1);
    ui->InputToAddTask->clear();
    ui->taskSetting->setCurrentIndex(0);

}
//设置优先级
void MainWindow::setPriority(int index)
{
    task->m_priority=index;
    task->setPriority();
    switch(index){
        case 1:
            task->m_item->setIcon(0,QIcon(":/icons/icon/lowPriority.png"));
            break;
        case 2:
            task->m_item->setIcon(0,QIcon(":/icons/icon/midPriority.png"));
            break;
        case 3:
            task->m_item->setIcon(0,QIcon(":/icons/icon/highPriority.png"));
            break;
    }
}
//设置note
void MainWindow::setNote()
{
    ui->setNote->setEnabled(false);
    bool ok;
    QString text = QInputDialog::getText(this, tr("备注"),
                                              tr("备注:"), QLineEdit::Normal,
                                              QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty())
         qDebug()<<text;
    ui->setNote->setEnabled(true);
    if(!text.isEmpty()){
        task->m_note=text;
        task->setNote();
    }
    refresh();
    onRefreshButtonClicked();

}
//设置描述
void MainWindow::setDescription()
{
    ui->setDescription->setEnabled(false);
    bool ok;
    QString text = QInputDialog::getMultiLineText(this, tr("备注"),
                                              tr("备注:"), "",&ok);
    if (ok && !text.isEmpty())
         qDebug()<<text;
    ui->setDescription->setEnabled(true);
    if(!text.isEmpty()){
        task->m_description=text;
        task->setDescription();
    }
    refresh();
    onRefreshButtonClicked();
}
//设置提醒时间
void MainWindow::setRemindTime()
{
    chooseRemindDateTimeWindow=new chooseRemindDateTime(0,this);
    chooseRemindDateTimeWindow->setAttribute(Qt::WA_ShowModal,true);
    chooseRemindDateTimeWindow->setAttribute(Qt::WA_DeleteOnClose,true);//避免内存泄露
    chooseRemindDateTimeWindow->show();
}
//设置到期时间
void MainWindow::setDueTime()
{
    chooseDueDateWindow=new ChooseDueDate(0,this);
    chooseDueDateWindow->setAttribute(Qt::WA_ShowModal,true);
    chooseDueDateWindow->setAttribute(Qt::WA_DeleteOnClose,true);
    chooseDueDateWindow->show();
}

void MainWindow::setDone()
{
    if(ui->taskTitle->isChecked())
    {
        task->setDone();
        refresh();
        onRefreshButtonClicked();
        ui->taskSetting->setCurrentIndex(0);
    }
}
/*****************************************我的待办************************************/
void MainWindow::onMyTaskPageButtonClicked()
{
    curGroupId=1;
    ui->sortBox->setVisible(true);
    ui->groupRefreshButton->setVisible(true);
    ui->refreshButton->setVisible(false);
    ui->label->setText("我的待办");
    ui->stackTaskList->setCurrentIndex(1);
    ui->taskSetting->setCurrentIndex(0);
    refresh();

}
void MainWindow::onMyFrontPageButtonClicked()
{
    curGroupId=1;
    ui->sortBox->setVisible(false);
    ui->groupRefreshButton->setVisible(false);
    ui->refreshButton->setVisible(true);
    ui->label->setText("我的首页");
    ui->stackTaskList->setCurrentIndex(0);
    ui->taskSetting->setCurrentIndex(0);
}
/*****************************************我的分组************************************/
void MainWindow::addGroupOrChangePage(QListWidgetItem *item)
{

    ui->taskSetting->setCurrentIndex(0);
    ui->stackTaskList->setCurrentIndex(1);
    qDebug()<<item->data(Qt::UserRole).toInt();
    if(item->data(Qt::UserRole).toInt()==0){//点击“添加分组”
        ui->label->setText("添加分组");
        bool ok;
        QString text = QInputDialog::getText(this, tr("新建待办分组"),
                                                  tr("分组名:"), QLineEdit::Normal,
                                                  QDir::home().dirName(), &ok);
        bool temp=group->canBeAdded(text);
        if (temp&& ok && !text.isEmpty()){
            group->m_groupName=text;
            int id=group->addGroup();

            QListWidgetItem* newitem=new QListWidgetItem();
            newitem->setText(text);
            newitem->setData(Qt::UserRole,id);
            newitem->setIcon(QIcon(":/icons/icon/myGroup.png"));
            ui->groupList->addItem(newitem);
            switchGroup(item);//????
            curGroupId=1;

        }

    }
    else
        switchGroup(item);
}
void MainWindow::switchGroup(QListWidgetItem* item)
{
    ui->sortBox->setVisible(true);
    ui->groupRefreshButton->setVisible(true);
    ui->refreshButton->setVisible(false);//更换按钮组

    curGroupId=item->data(Qt::UserRole).toInt();
    QString tableName=group->getGroupNameById();
    ui->label->setText(tableName);
    refresh();
}
void MainWindow::refresh()
{
    for(int i=0;i<2;++i)
        ui->taskList_2->topLevelItem(i)->takeChildren();//删除顶层节点的所有子节点
      QString tableName=group->getGroupNameById();
      database.open();
      QSqlQuery query;
      QString str=QString("select * from '%1'").arg(tableName);
      //qDebug()<<str;
      query.exec(str);
      int level=0;
      while(query.next()){
         if(query.value(8).toInt()==1)
             level=1;
         QTreeWidgetItem *item = new QTreeWidgetItem(ui->taskList_2->topLevelItem(level));
         qDebug()<<item->parent();
         item->setText(0,query.value(1).toString());
         //qDebug()<<query.value(1).toString();
         //分为没做完和做完考虑
         if(!level){
             item->setCheckState(0,Qt::Unchecked);
             QString temp;
             if(!(temp=query.value(3).toString()).isEmpty())
                 item->setText(1,temp);
             if(!(temp=query.value(4).toString()).isEmpty()){
                 item->setText(2,temp);
                 item->setIcon(2,QIcon(":/icons/icon/setRemindTime.png"));
             }
             if(!(temp=query.value(2).toString()).isEmpty()){
                 item->setText(3,temp);
             }
             if(!(temp=query.value(5).toString()).isEmpty())
             {
                 item->setText(4,temp);
             }
             if(!(temp=query.value(6).toString()).isEmpty())
             {
                 item->setText(5,temp);
             }
         }
         else{
             item->setCheckState(0,Qt::Checked);
         }
        //设置id
         item->setData(0,Qt::UserRole,query.value(0).toInt());
        //设置优先级图标
         switch(query.value(7).toInt()){
             case 1:
                 item->setIcon(0,QIcon(":/icons/icon/lowPriority.png"));
                 break;
             case 2:
                 item->setIcon(0,QIcon(":/icons/icon/midPriority.png"));
                 break;
             case 3:
                 item->setIcon(0,QIcon(":/icons/icon/highPriority.png"));
                 break;
         }
         level=0;
    }
       ui->taskList_2->topLevelItem(0)->setExpanded(1);

}
void MainWindow::sort(int index){

    database.open();
    QSqlQuery query;
    QString tableName=group->getGroupNameById();
    bool ok;
    //到期1 创建2 优先级3
    switch(index){
        case 1:
            ok=query.exec(QString("SELECT * FROM '%1' ORDER BY dueTime").arg(tableName));
            break;
        case 2:
            ok=query.exec(QString("SELECT * FROM '%1' ORDER BY createTime").arg(tableName));
            break;
        case 3:
            ok=query.exec(QString("SELECT * FROM '%1' ORDER BY priority DeSC").arg(tableName));
            break;

    }
    if(!ok) {
        qDebug() << query.executedQuery();
        qDebug() << query.lastError();
    }
    if(index!=0){
        for(int i=0;i<2;++i)
            ui->taskList_2->topLevelItem(i)->takeChildren();//删除顶层节点的所有子节点
        int level=0;
        while(query.next()){
           if(query.value(8).toInt()==1)
               level=1;
           QTreeWidgetItem *item = new QTreeWidgetItem(ui->taskList_2->topLevelItem(level));
           item->setText(0,query.value(1).toString());
           //qDebug()<<query.value(1).toString();
           //分为没做完和做完考虑
           if(!level){
               item->setCheckState(0,Qt::Unchecked);
               QString temp;
               if(!(temp=query.value(3).toString()).isEmpty())
                   item->setText(1,temp);
               if(!(temp=query.value(4).toString()).isEmpty()){
                   item->setText(2,temp);
                   item->setIcon(2,QIcon(":/icons/icon/setRemindTime.png"));
               }
               if(!(temp=query.value(2).toString()).isEmpty()){
                   item->setText(3,temp);
               }
               if(!(temp=query.value(5).toString()).isEmpty())
               {
                   item->setText(4,temp);
               }
               if(!(temp=query.value(6).toString()).isEmpty())
               {
                   item->setText(5,temp);
               }
           }
           else{
               item->setCheckState(0,Qt::Checked);
           }
          //设置id
           item->setData(0,Qt::UserRole,query.value(0).toInt());
          //设置优先级图标
           switch(query.value(7).toInt()){
               case 1:
                   item->setIcon(0,QIcon(":/icons/icon/lowPriority.png"));
                   break;
               case 2:
                   item->setIcon(0,QIcon(":/icons/icon/midPriority.png"));
                   break;
               case 3:
                   item->setIcon(0,QIcon(":/icons/icon/highPriority.png"));
                   break;
           }
           level=0;
      }
    }
    ui->taskList_2->topLevelItem(0)->setExpanded(1);


    return;
}

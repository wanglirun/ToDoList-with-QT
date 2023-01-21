#include "choosereminddatetime.h"
#include "ui_choosereminddatetime.h"

chooseRemindDateTime::chooseRemindDateTime(QWidget *parent,MainWindow* main) :
    QWidget(parent),
    ui(new Ui::chooseRemindDateTime)
{
    ui->setupUi(this);
    this->main=main;
    connect(ui->chooseRemindDateEdit,&QCalendarWidget::clicked,this,enableButton);
    connect(ui->chooseRemindDateEdit,&QCalendarWidget::currentPageChanged,this,disableButton);
    connect(ui->chooseRemindTimeSureButton,&QPushButton::clicked,this,getDateTime);
    connect(this,&chooseRemindDateTime::closeWindow,this,&chooseRemindDateTime::close);
    connect(ui->removeRemindTime,&QPushButton::clicked,
            this,&chooseRemindDateTime::removeRemindTime);
}

chooseRemindDateTime::~chooseRemindDateTime()
{
    delete ui;
}

void chooseRemindDateTime::enableButton()
{
    ui->chooseRemindTimeSureButton ->setEnabled(true);
}
void chooseRemindDateTime::disableButton()
{
    ui->chooseRemindTimeSureButton->setEnabled(false);
}
void chooseRemindDateTime::getDateTime()
{
    QString date=ui->chooseRemindDateEdit->selectedDate().toString("yyyy-MM-dd ");
    QString time=ui->chooseRemindTimeEdit->time().toString("hh:mm");
    main->task->m_remindTime=date+time;
    main->task->setRemindTime();
    if(main->curGroupId==1){
        main->refresh();
        main->onRefreshButtonClicked();
    }
    else
        main->refresh();
    emit closeWindow();
}
void chooseRemindDateTime::removeRemindTime()
{
    main->task->m_remindTime=QString();
    main->task->setRemindTime();
    if(main->curGroupId==1){
        main->refresh();
        main->onRefreshButtonClicked();
    }
    else
        main->refresh();
    emit closeWindow();
}

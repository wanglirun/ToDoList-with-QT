#include "choosedate.h"
#include "ui_choosedate.h"

ChooseDate::ChooseDate(QWidget *parent,MainWindow *main) :
    QWidget(parent),
    ui(new Ui::ChooseDate)
{
    ui->setupUi(this);
    this->main=main;
    ui->dueSureButton->setEnabled(false);
    ui->remindSureButton->setEnabled(false);

    ui->tabWidget->setCurrentIndex(0);
    connect(ui->selectDueDate,&QCalendarWidget::clicked,this,enableButton);
    connect(ui->remindDateEdit,&QCalendarWidget::clicked,this,enableRemindButton);

    connect(ui->selectDueDate,&QCalendarWidget::currentPageChanged,this,disableButton);
    connect(ui->remindDateEdit,&QCalendarWidget::currentPageChanged,this,disableRemindButton);

    connect(ui->dueSureButton,&QPushButton::clicked,this,getDate);
    connect(this,&ChooseDate::closeWindow,this,&ChooseDate::close);
    connect(ui->dueCancelButton,&QPushButton::clicked,this,&ChooseDate::close);
    connect(ui->remindCancelButton,&QPushButton::clicked,this,&ChooseDate::close);
    connect(ui->remindSureButton,&QPushButton::clicked,this,&ChooseDate::getRemindTime);
}


ChooseDate::~ChooseDate()
{
    delete ui;
}

void ChooseDate::enableButton()
{
    ui->dueSureButton->setEnabled(true);
    //ui->remindSureButton->setEnabled(true);
}
void ChooseDate::disableButton()
{
    ui->dueSureButton->setEnabled(false);
    //ui->remindSureButton->setEnabled(false);
}
void ChooseDate::enableRemindButton()
{
    //ui->dueSureButton->setEnabled(true);
    ui->remindSureButton->setEnabled(true);
}
void ChooseDate::disableRemindButton()
{
    //ui->dueSureButton->setEnabled(false);
    ui->remindSureButton->setEnabled(false);
}
void ChooseDate::getDate()
{
    if(ui->remindSureButton->isEnabled()){
        QString date=ui->remindDateEdit->selectedDate().toString("yyyy-MM-dd ");
        QString time=ui->RemindTimeEdit->time().toString("hh:mm");
        main->task->m_remindTime=date+time;
    }
    QDate date=ui->selectDueDate->selectedDate();
    main->task->m_dueTime=date.toString("yyyy-MM-dd");
    emit closeWindow();
}
void ChooseDate::getRemindTime()
{
    if(ui->dueSureButton->isEnabled())
    {
        QDate date=ui->selectDueDate->selectedDate();
        main->task->m_dueTime=date.toString("yyyy-MM-dd");
    }
    QString date=ui->remindDateEdit->selectedDate().toString("yyyy-MM-dd ");
    QString time=ui->RemindTimeEdit->time().toString("hh:mm");
    main->task->m_remindTime=date+time;
    emit closeWindow();
}

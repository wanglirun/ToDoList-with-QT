#include "chooseduedate.h"
#include "ui_chooseduedate.h"

ChooseDueDate::ChooseDueDate(QWidget *parent,MainWindow* main) :
    QWidget(parent),
    ui(new Ui::ChooseDueDate)
{
    ui->setupUi(this);
    this->main=main;
    connect(ui->chooseDueDateEdit,&QCalendarWidget::clicked,this,enableButton);
    connect(ui->chooseDueDateEdit,&QCalendarWidget::currentPageChanged,this,disableButton);
    connect(ui->chooseDueDateSureButton,&QPushButton::clicked,this,getDate);
    connect(this,&ChooseDueDate::closeWindow,this,&ChooseDueDate::close);
    connect(ui->removeDueDate,&QPushButton::clicked,
            this,&ChooseDueDate::removeDueDate);
}

ChooseDueDate::~ChooseDueDate()
{
    delete ui;
}
void ChooseDueDate::enableButton()
{
    ui->chooseDueDateSureButton->setEnabled(true);
}
void ChooseDueDate::disableButton()
{
    ui->chooseDueDateSureButton->setEnabled(false);
}
void ChooseDueDate::getDate()
{
    main->task->m_dueTime=ui->chooseDueDateEdit->selectedDate().toString("yyyy-MM-dd");
    main->task->setDueTime();
    if(main->curGroupId==1){
        main->onRefreshButtonClicked();
        main->refresh();
    }
    else
        main->refresh();

    emit closeWindow();
}
void ChooseDueDate::removeDueDate()
{
    main->task->m_dueTime=QString();
    main->task->setDueTime();
    if(main->curGroupId==1){
        main->refresh();
        main->onRefreshButtonClicked();
    }
    else
        main->refresh();
    emit closeWindow();
}

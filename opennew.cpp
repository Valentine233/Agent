#include "opennew.h"

QDateTime MainWindow::curr_time;
QList<Event*>* MainWindow::mylist;
QList<Event*>* MainWindow::yourlist;
void AddEvent(QString, QString, QString, QString);

OpenNew::OpenNew(QWidget* parent):QDialog(parent)
{
    setWindowTitle("New");
}

void OpenNew::setInit(int x, int y)
{
    //qDebug() << x << y;
    // y 5 for 15 min, 20 for 1 hour， a grid is 40 - 2 hours
    QTime* eventStartTime = new QTime((y- MainWindow::topY)/(MainWindow::gridHight/2), (y- MainWindow::topY)%(MainWindow::gridHight/2)/((MainWindow::gridHight/4))*30);
    QString weekStrings[7] = {"周一","周二","周三","周四","周五","周六","周日"};
    QString currDay = MainWindow::curr_time.toString("ddd");
    int d;
    for (d=0;d<7;d++) {
        if (weekStrings[d] == currDay)
            break;
    }
    QDate eventDay = MainWindow::curr_time.date().addDays((x-MainWindow::leftX)/(MainWindow::gridWidth*2)-d);
    QLabel* name = new QLabel(this);
    name->setText("事件名称：");
    name->setGeometry(QRect(20,10,60,20));

    nameinput->setGeometry(QRect(100,10,100,20));

    QLabel* place = new QLabel(this);
    place->setText("事件地点：");
    place->setGeometry(QRect(20,40,60,20));

    placeinput->setGeometry(QRect(100,40,100,20));

    QLabel* time = new QLabel(this);
    time->setText("时间类型：");
    time->setGeometry(QRect(20,70,60,20));

    QString timetype[] = {"单次事件","重复事件"};
    QButtonGroup* timechoise = new QButtonGroup(this);
    for(int i=0; i<2 ; i++)
    {
        QRadioButton* choise = new QRadioButton(this);
        choise->setText(timetype[i]);
        choise->setGeometry(100+100*i,70,100,20);
        timechoise->addButton(choise,i+1);
    }
    timechoise->button(1)->click();

    //单次日期
//    dateEdit->setMinimumDate(QDate::currentDate().addDays(-365));
//    dateEdit->setMaximumDate(QDate::currentDate().addDays(365));
    dateEdit->setDate(eventDay);
    dateEdit->setDisplayFormat("yyyy/MM/dd");
    dateEdit->setGeometry(20,105,150,25);

    //重复日期

    start1->setText("从");
    start1->setGeometry(QRect(20,108,20,20));
    start1->hide();

    startdate->setMinimumDate(QDate::currentDate());
    startdate->setMaximumDate(QDate::currentDate().addDays(365));
    startdate->setDisplayFormat("yyyy/MM/dd");
    startdate->setGeometry(40,105,150,25);
    startdate->hide();

    end1->setText("至");
    end1->setGeometry(QRect(200,108,20,20));
    end1->hide();

    enddate->setMinimumDate(QDate::currentDate());
    enddate->setMaximumDate(QDate::currentDate().addDays(365));
    enddate->setDisplayFormat("yyyy/MM/dd");
    enddate->setGeometry(220,105,150,25);
    enddate->hide();

    //时间
    QLabel* start2 = new QLabel(this);
    start2->setText("从");
    start2->setGeometry(QRect(20,143,20,20));

//    starttime->setMinimumTime(QTime::currentTime().addSecs(-3600*24));
//    starttime->setMaximumTime(QTime::currentTime().addSecs(3600*24));
    starttime->setTime(*eventStartTime);
    starttime->setDisplayFormat("HH:mm");
    starttime->setGeometry(40,140,150,25);

    QLabel* end2 = new QLabel(this);
    end2->setText("至");
    end2->setGeometry(QRect(200,143,20,20));

    endtime->setTime(eventStartTime->addSecs(60*Event::defaultDuration));
//    endtime->setMinimumTime(QTime::currentTime().addSecs(-3600*24));
//    endtime->setMaximumTime(QTime::currentTime().addSecs(3600*24));
    endtime->setDisplayFormat("HH:mm");
    endtime->setGeometry(220,140,150,25);

    //week选项
//    QString weekStrings[7] = {"周一", "周二", "周三", "周四", "周五", "周六", "周日"};
    QButtonGroup *weekdays = new QButtonGroup;
    for(int i=0; i<7; i++)
    {
        QCheckBox* weekday = new QCheckBox(weekStrings[i], this);
        weekday->setGeometry(20+50*i,180,50,20);
        weekdays->addButton(weekday);

    }

    addButton->setText("添加");
    addButton->setGeometry(QRect(300,210,40,20));
    addButton->setStyleSheet("QPushButton {color: black;}");

    QObject::connect(timechoise, SIGNAL(buttonPressed(int)), this, SLOT(TimeChoose(int)));
    QObject::connect(addButton, SIGNAL(clicked(bool)), this, SLOT(sendAdd()));
    QObject::connect(this, SIGNAL(transAdd(QString, QString, QDateTime, QDateTime, int)), this->parent(),
                     SLOT(createNewEvent(QString,QString,QDateTime, QDateTime, int)));
    QObject::connect(this, SIGNAL(transAdd(QString, QString, QDateTime, QDateTime, int)), this, SLOT(close()));

    confirmButton->setText("确认");
    confirmButton->setGeometry(QRect(300,210,40,20));
    confirmButton->setStyleSheet("QPushButton {color: black;}");
    confirmButton->hide();

    QObject::connect(confirmButton, SIGNAL(clicked(bool)), this, SLOT(sendEdit()));
    QObject::connect(this, SIGNAL(transEdit(QString, QString, QDateTime, QDateTime, int, QString, QString, QDateTime, QDateTime)),
                     this->parent(), SLOT(editEvent(QString, QString, QDateTime, QDateTime, int, QString, QString, QDateTime, QDateTime)));
    QObject::connect(this, SIGNAL(transEdit(QString, QString, QDateTime, QDateTime, int, QString, QString, QDateTime, QDateTime)),
                     this, SLOT(close()));

    deleteButton->setText("删除");
    deleteButton->setGeometry(QRect(200,210,40,20));
    deleteButton->setStyleSheet("QPushButton {color: black;}");
    deleteButton->hide();

    QObject::connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(sendDelete()));
    QObject::connect(this, SIGNAL(transDelete(QString,QString,QDateTime,QDateTime,int)),
                     this, SLOT(deleteEventConfirm(QString, QString, QDateTime, QDateTime, int)));
    QObject::connect(this, SIGNAL(deleteConfirm(QString,QString,QDateTime,QDateTime,int)),
                     this->parent(), SLOT(deleteEvent(QString, QString, QDateTime, QDateTime, int)));
    QObject::connect(this, SIGNAL(deleteConfirm(QString,QString,QDateTime,QDateTime,int)),
                     this, SLOT(close()));

    // build a temp event
    Event *tempEvent = new Event("未命名","",QDateTime(dateEdit->date(), starttime->dateTime().time()),QDateTime(dateEdit->date(), endtime->dateTime().time()),0,this);
    ((MainWindow*)this->parent())->tempUI = ((MainWindow*)this->parent())->addEventUI(tempEvent);
}

void OpenNew::sendAdd() //myevent: type = 0,   yourevent: type = 1
{
    QDateTime start(dateEdit->date(), starttime->dateTime().time());
    QDateTime end(dateEdit->date(), endtime->dateTime().time());
    deleteTemp();
    emit transAdd(nameinput->text(), placeinput->text(), start, end, 0);
}

void OpenNew::sendEdit()
{
    QDateTime start(dateEdit->date(), starttime->dateTime().time());
    QDateTime end(dateEdit->date(), endtime->dateTime().time());

    QDateTime startOld(dateOld, starttimeOld);
    QDateTime endOld(dateOld, endtimeOld);
    emit transEdit(nameinput->text(), placeinput->text(), start, end, 0, nameOld, placeOld, startOld, endOld);
}

void OpenNew::sendDelete()
{
    QDateTime start(dateEdit->date(), starttime->dateTime().time());
    QDateTime end(dateEdit->date(), endtime->dateTime().time());
    emit transDelete(nameinput->text(), placeinput->text(), start, end, 0);
}

void OpenNew::deleteTemp()
{
    if (((MainWindow*)this->parent())->tempUI != NULL) {
        delete ((MainWindow*)this->parent())->tempUI;
        ((MainWindow*)this->parent())->tempUI = NULL;
    }
}

void OpenNew::closeEvent(QCloseEvent *event) {
    deleteTemp();
}

void OpenNew::TimeChoose(int id)
{

    if(id==1) //单次事件
    {
        startdate->hide();
        enddate->hide();
        start1->hide();
        end1->hide();
        dateEdit->show();
        starttime->show();
        endtime->show();
    }
    else if(id==2) //重复事件
    {
        dateEdit->hide();
        startdate->show();
        enddate->show();
        start1->show();
        end1->show();
    }

}

void OpenNew::showCurr(Event *event)
{
    qDebug() << event->eventName << event->eventPlace  << event->eventStart.date() << event->eventStart.time()<<"\n";
    setWindowTitle("Edit");
    qDebug() << "1" << "\n";
    nameinput->setText(event->eventName);
    nameOld = event->eventName;
    placeinput->setText(event->eventPlace);
    placeOld = event->eventPlace;
    dateEdit->setDate(event->eventStart.date());
    dateOld = event->eventStart.date();
    starttime->setTime(event->eventStart.time());
    starttimeOld = event->eventStart.time();
    endtime->setTime(event->eventEnd.time());
    endtimeOld = event->eventEnd.time();
    qDebug() << "2" << "\n";
    addButton->hide();
    confirmButton->show();
    deleteButton->show();
    this->exec();

}

void deleteEventConfirm(QString, QString, QDateTime, QDateTime, int)
{
    QMessageBox::information(NULL, "Delete", "Are you sure to delete the event?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}


#include "createnew.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDateTimeEdit>

CreateNew::CreateNew(QWidget* parent):QDialog(parent)
{
    setWindowTitle("create new event");
}

void CreateNew::setinit()
{
    QLabel *day_c = new QLabel(this);
    day_c->setGeometry(QRect(100, 200, 100, 30));
    day_c->setText("Day: ");

    day = new QLineEdit(this);
    day->setGeometry(200,200,100,30);

//    QComboBox *day = new QComboBox(this);
//    day->setGeometry(200,200,100,30);
//    day->addItem(QWidget::tr("Monday"));
//    day->addItem(QWidget::tr("Tuesday"));
//    day->addItem(QWidget::tr("Wednesday"));
//    day->addItem(QWidget::tr("Thursday"));
//    day->addItem(QWidget::tr("Friday"));
//    day->addItem(QWidget::tr("Saturday"));
//    day->addItem(QWidget::tr("Sunday"));

    QLabel *time_c = new QLabel(this);
    time_c->setGeometry(QRect(100,250,100,30));
    time_c->setText("Time: ");


    time1 = new QLineEdit(this);
    time1->setGeometry(200,250,100,30);

    QLabel *time_m = new QLabel(this);
    time_m->setGeometry(300,250,10,30);
    time_m->setText(" - ");

    time2 = new QLineEdit(this);
    time2->setGeometry(310,250,100,30);

    QPushButton *confirmButton = new QPushButton ( "&OK", this);
    confirmButton->setGeometry(400,400,80,40);
    confirmButton->setStyleSheet("QPushButton {color: black;}");
    QObject::connect(confirmButton, SIGNAL(clicked(bool)), SLOT(send()));
}


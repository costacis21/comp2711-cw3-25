#ifndef REPEAT_BUTTON_H
#define REPEAT_BUTTON_H
#include <QPushButton>
#include <QIcon>



class RepeatButton : public QPushButton {
    Q_OBJECT

public:
    bool state=true;//true for repeat On
     RepeatButton(QWidget *parent) :  QPushButton(parent) {
         setIcon(QIcon(":/icons/repeatOn.png"));
         QWidget::setFixedSize(QSize(50,50));
         connect(this,SIGNAL(clicked()),this,SLOT(repeatChanged()));
     }

     void init();


public slots:
    void repeatChanged();


};

#endif // REPEAT_BUTTON_H

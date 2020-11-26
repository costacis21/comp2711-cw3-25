#ifndef BACKWARD_BUTTON_H
#define BACKWARD_BUTTON_H

#include <QPushButton>
#include <QIcon>

class BackwardButton : public QPushButton {
    Q_OBJECT

public:

     BackwardButton(QWidget *parent) :  QPushButton(parent) {
         setIcon(QIcon(":/icons/backward.png"));
         QWidget::setFixedSize(QSize(50,50));
     }

     void init();


//signals:
//    inline void movebackward();


};
#endif // BACKWARD_BUTTON_H

#ifndef FORWARD_BUTTON_H
#define FORWARD_BUTTON_H


#include <QPushButton>
#include <QIcon>



class ForwardButton : public QPushButton {
    Q_OBJECT

public:

     ForwardButton(QWidget *parent) :  QPushButton(parent) {
         setIcon(QIcon(":/icons/forward.png"));
         QWidget::setFixedSize(QSize(50,50));
     }

     void init();


//signals:
//    inline void moveforward();


};



#endif // FORWARD_BUTTON_H

#ifndef FORWARD_BUTTON_H
#define FORWARD_BUTTON_H


#include <QPushButton>
#include <QIcon>



class ForwardButton : public QPushButton {
    Q_OBJECT

public:

     ForwardButton(QWidget *parent) :  QPushButton(parent) {
         setIcon(QIcon(":/icons/next.png"));
         setIconSize(QSize(50,50));

         setFlat(true);

     }

     void init();





};



#endif // FORWARD_BUTTON_H

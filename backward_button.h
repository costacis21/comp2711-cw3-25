#ifndef BACKWARD_BUTTON_H
#define BACKWARD_BUTTON_H

#include <QPushButton>
#include <QIcon>

class BackwardButton : public QPushButton {
    Q_OBJECT

public:

     BackwardButton(QWidget *parent) :  QPushButton(parent) {
         setIcon(QIcon(":/icons/previous.png"));
         setIconSize(QSize(50,50));
         setFlat(true);
         setFocusPolicy(Qt::NoFocus);
         setCursor(QCursor(Qt::PointingHandCursor));


     }

     void init();




};
#endif // BACKWARD_BUTTON_H

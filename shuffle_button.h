#ifndef SHUFFLE_BUTTON_H
#define SHUFFLE_BUTTON_H
#include <QPushButton>

class ShuffleButton : public QPushButton {
    Q_OBJECT

public:
    bool state=true;//true for repeat On
     ShuffleButton(QWidget *parent) :  QPushButton(parent) {
         setIcon(QIcon(":/icons/shuffleOn.png"));
//         QWidget::setFixedSize(QSize(50,50));
         setIconSize(QSize(50,50));

         connect(this,SIGNAL(clicked()),this,SLOT(repeatChanged()));
         setFlat(true);

     }

     void init();


public slots:
    void repeatChanged();


};

#endif // SHUFFLE_BUTTON_H

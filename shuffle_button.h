#ifndef SHUFFLE_BUTTON_H
#define SHUFFLE_BUTTON_H
#include <QPushButton>

class ShuffleButton : public QPushButton {
    Q_OBJECT

public:
    bool state=true;//true for repeat On
     ShuffleButton(QWidget *parent) :  QPushButton(parent) {
         setIcon(QIcon(":/icons/shuffleOn.png"));
         setIconSize(QSize(50,50));
         setFlat(true);
         connect(this,SIGNAL(clicked()),this,SLOT(repeatChanged()));
     }

     void init();


public slots:
    void repeatChanged();


};

#endif // SHUFFLE_BUTTON_H

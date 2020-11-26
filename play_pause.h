#ifndef PLAY_PAUSE_H
#define PLAY_PAUSE_H

#include <QPushButton>
#include <QIcon>
#include "the_player.h"




class playpauseButton : public QPushButton {
    Q_OBJECT
    bool state;//true for play

public:

     playpauseButton(QWidget *parent) :  QPushButton(parent) {
         setIcon(QIcon(":/icons/pause.png"));
         setIconSize(QSize(50,50));
         connect(this, SIGNAL(clicked()), this, SLOT (playpauseclicked())); // if QPushButton clicked...then run playpauseclicked() below

     }

     void init();



private slots:
     void playpauseclicked();

signals:
    inline void changeplayStatus();


};


#endif // PLAY_PAUSE_H

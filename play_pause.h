#ifndef PLAY_PAUSE_H
#define PLAY_PAUSE_H

#include <QKeyEvent>
#include <QPushButton>
#include <QIcon>
#include "the_player.h"




class PlaypauseButton : public QPushButton {
    Q_OBJECT
    bool state=false;//true for play

public:

     PlaypauseButton(QWidget *parent) :  QPushButton(parent) {
         setIcon(QIcon(":/icons/pause.png"));
         setIconSize(QSize(50,50));
         setFlat(true);
         connect(this, SIGNAL(clicked()), this, SLOT (playpauseclicked())); // if QPushButton clicked...then run playpauseclicked() below


     }
     virtual void keyPressEvent(QKeyEvent* event) override;

     void init();



private slots:
     void playpauseclicked();

signals:
    void changeplayStatus();


};


#endif // PLAY_PAUSE_H

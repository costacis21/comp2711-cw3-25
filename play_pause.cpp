//
// Created by twak on 11/11/2019.
//

#include "play_pause.h"

//true for play


void playpauseButton::playpauseclicked() {

    if (state){
        this->setIcon(QIcon(":/icons/pause.png"));
    }

    else{
        this->setIcon(QIcon(":/icons/play.png"));
    }

    state=!state;

}




void playpauseButton::keyPressEvent(QKeyEvent* event){
        if ( event->key() == Qt::Key_Space) {
            playpauseclicked();
            emit changeplayStatus();
        }
}

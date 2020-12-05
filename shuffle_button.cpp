#include "shuffle_button.h"

void ShuffleButton::repeatChanged(){
    if(this->state)
        this->setIcon(QIcon(":/icons/shuffleOff.png"));
    else
        this->setIcon(QIcon(":/icons/shuffleOn.png"));
    this->state=!this->state;

}

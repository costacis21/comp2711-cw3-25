#include "shuffle_button.h"

void ShuffleButton::repeatChanged(){
    //todo change colour
    if(this->state)
        this->setIcon(QIcon(":/icons/shuffleOff.png"));
    else
        this->setIcon(QIcon(":/icons/shuffleOn.png"));
    this->state=!this->state;

}

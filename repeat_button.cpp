#include "repeat_button.h"

void RepeatButton::repeatChanged(){
    if(this->state)
        this->setIcon(QIcon(":/icons/repeatOff.png"));
    else
        this->setIcon(QIcon(":/icons/repeatOn.png"));
    this->state=!this->state;

}

#include "time_slider.h"


void TimeSlider::setRange(int t){
    this->setMinimum(0);
    this->setMaximum(t);
}

void TimeSlider::setValueqint(qint64 t){
    this->setValue(int(t));
}

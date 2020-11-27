#ifndef TIME_SLIDER_H
#define TIME_SLIDER_H
#include <QSlider>

class TimeSlider : public QSlider {
    Q_OBJECT

public:

     TimeSlider(QWidget *parent) :  QSlider(parent) {
         this->setOrientation(Qt::Horizontal);
//         this->minimumSizeHint()=QSize(0,100);
     }

     void init();

public slots:
     void setValueqint(qint64);


signals:
    inline void setRange(int);


};
#endif // TIME_SLIDER_H

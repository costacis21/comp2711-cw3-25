#ifndef MY_VIDEOWIDGET_H
#define MY_VIDEOWIDGET_H

#include <QKeyEvent>
#include <QVideoWidget>

class MyVideoWidget : public QVideoWidget {

Q_OBJECT
    public:
        MyVideoWidget(QWidget* parent = nullptr) : QVideoWidget(parent) {
            setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
        }



protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;


};


#endif // MY_VIDEOWIDGET_H

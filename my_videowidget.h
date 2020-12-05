#ifndef MY_VIDEOWIDGET_H
#define MY_VIDEOWIDGET_H

#include <QKeyEvent>
#include <QVideoWidget>
#include <QMessageBox>
#include <QTimer>

class MyVideoWidget : public QVideoWidget {

Q_OBJECT
    public:
        MyVideoWidget(QWidget* parent = nullptr) : QVideoWidget(parent) {
            setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
            QObject::connect(this, &MyVideoWidget::fullScreenChanged,[=]( const bool fullscreen ){


                if (fullscreen){
                    QTimer *mTimer = new QTimer(NULL);
                    mTimer->setInterval(2500); // 2000ms is two seconds between ...
                    mTimer->start();
                    QMessageBox *helpBox = new QMessageBox();
                    helpBox->setText("Double click or Press esc key\nTo exit Fullscreen");
                    helpBox->setStandardButtons(QMessageBox::NoButton);
                    helpBox->setStyleSheet("font: 20pt; font-weight: bold; align: center;");
                    helpBox->setAttribute(Qt::WA_TranslucentBackground);
                    helpBox->setWindowFlag(Qt::FramelessWindowHint);
                    helpBox->show();
                    connect( mTimer, &QTimer::timeout, [=](){
                        helpBox->hide();
                    } ); // ...popmessage for 2 sec
                }



            } );
        }




protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void pauseChanged();

};


#endif // MY_VIDEOWIDGET_H

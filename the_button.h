//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_BUTTON_H
#define CW2_THE_BUTTON_H


#include <QPushButton>
#include <QUrl>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QMediaMetaData>


class TheButtonInfo {


public:
    QUrl* url; // video file to play
    QIcon* icon; // icon to display
    QString name;
    QString location;
    QString duration;

    TheButtonInfo ( QUrl* url, QIcon* icon) : url (url), icon (icon) {
        name=getName();

}

private:
    QString miliToFormated(qint64 time);
    QString getName();
    QString getLocation();
    QString getDuration();

};

class TheButton : public QPushButton {
    Q_OBJECT

public:
    TheButtonInfo* info;

     TheButton(QWidget *parent) :  QPushButton(parent) {
         setIconSize(QSize(250,160));
         connect(this, SIGNAL(released()), this, SLOT (clicked())); // if QPushButton clicked...then run clicked() below
         setFocusPolicy(Qt::NoFocus);
    }

    void init(TheButtonInfo* i);

private slots:
    void clicked();

signals:
    void jumpTo(TheButtonInfo*);

};

#endif //CW2_THE_BUTTON_H

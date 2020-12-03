//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include <vector>
#include <QTimer>

using namespace std;

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;
    QTimer* mTimer;
    long updateCount = 0;
    int currVid=0;

public:
    bool playstatus=true;//true for play
    bool repeatOn=true;
    bool shuffleOn=true;
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0); // be slightly less annoying
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );
        connect(this,SIGNAL(durationChanged(qint64)),this,SLOT( timedurationS(qint64 )));

    }

    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

private slots:

    // change the image and video for one button every one second
    void shuffle();

    void playStateChanged (QMediaPlayer::State ms);


public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    void setVolumeSlot(int vol);
    void setSpeed(int speed);
    void changePlayPause();
    void forward();
    void backwards();
    void timedurationS(qint64 );
    void changeRepeat();
    void seek(int);


signals:
    void namechange(QString);
    void timeduration(QString);
    void timeS(int,int);
};

#endif //CW2_THE_PLAYER_H

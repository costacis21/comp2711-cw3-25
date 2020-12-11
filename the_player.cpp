//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include <stdio.h>


using namespace std;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
}

// change the image and video for one button every one second
void ThePlayer::shuffle() {
    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
//        setMedia(*i->url);
    buttons -> at( updateCount++ % buttons->size() ) -> init( i );
}

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {

    switch (ms) {
        case QMediaPlayer::State::StoppedState:
                if(repeatOn){
                    setPosition(0);
                    play(); // starting playing again...
                }
                else if(shuffleOn){
                    setMedia(*buttons -> at( updateCount++ % buttons->size())->info->url);
                    play();

                }else
                    stop();

            break;
    default:
        break;
    }


}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);

    emit namechange(button->name);
    play();


}

void ThePlayer::setVolumeSlot(int vol){
    setVolume(vol);
}

void ThePlayer::setSpeed(int speed){
    setPlaybackRate((qreal)speed*0.2);
}
void ThePlayer::changePlayPause(){
    if (this->playstatus)
        pause();
    else
        play();
    playstatus=!playstatus;

}

void ThePlayer::forward(){
    currVidIndex++;

    if(currVidIndex>buttons->size()-1)
        currVidIndex=0;
    jumpTo(buttons->at(currVidIndex)->info);
}

void ThePlayer::backwards(){
    currVidIndex--;

    if(currVidIndex<0)
        currVidIndex=buttons->size()-1;
    jumpTo(buttons->at(currVidIndex)->info);
}

void ThePlayer::timedurationS(qint64 time){
    long milli = time;
    //3600000 milliseconds in an hour
    long hr = milli / 3600000;
    milli = milli - 3600000 * hr;
    //60000 milliseconds in a minute
    long min = milli / 60000;
    milli = milli - 60000 * min;
    //1000 milliseconds in a second
    long sec = milli / 1000;
    milli = milli - 1000 * sec;
    QString formatedTime=QString(QString::number(min) + ":" + QString::number(sec) );

    if(hr==0)
        QString formatedTime=  QString(QString::number(min) + ":" + QString::number(sec) );
    else
        QString formatedTime=  QString(QString::number(hr) + ":"+QString::number(min) + ":" + QString::number(sec) );


    emit timeduration(formatedTime);
    emit timeS(0,int(time));
}

void ThePlayer::changeRepeat(){

    this->repeatOn=!this->repeatOn;

}

void ThePlayer::seek(int time){
    setPosition(qint64(time));

}





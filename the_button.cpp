//
// Created by twak on 11/11/2019.
//

#include "the_button.h"


QString TheButtonInfo::getName(){
        QString currUrl= url->toString();
        QStringList pieces = currUrl.split( "/" );
        QString neededWord = pieces.value( pieces.length() - 1 );
        pieces = neededWord.split( "." );
        neededWord = pieces.value( pieces.length()-2 );
        return neededWord;
}

QString TheButtonInfo::getLocation(){

    return location;
}

QString TheButtonInfo::miliToFormated(qint64 time){
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
    return formatedTime;
}

QString TheButtonInfo::getDuration(){
//    ThePlayer *tmpPlayer = new ThePlayer();
//    tmpPlayer->setMedia(*this->url);
    return duration;


}


void TheButton::init(TheButtonInfo* i) {
    setIcon( *(i->icon) );
    info =  i;
}


void TheButton::clicked() {
    emit jumpTo(info);
}



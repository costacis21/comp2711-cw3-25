//
// Created by twak on 11/11/2019.
//

#include "the_button.h"


QString TheButtonInfo::getNameFromURL(){
        QString currUrl= url->toString();
        QStringList pieces = currUrl.split( "/" );
        QString neededWord = pieces.value( pieces.length() - 1 );
        pieces = neededWord.split( "." );
        neededWord = pieces.value( pieces.length()-2 );
        return neededWord;
}

QString TheButtonInfo::getLocationFromUrl(){
    QMediaPlayer *tmpPlayer = new QMediaPlayer();
    QMediaPlaylist *playlist = new QMediaPlaylist();
    QMediaContent *tmpMediaContent = new  QMediaContent(*this->url);
    playlist->addMedia(*tmpMediaContent);
    tmpPlayer->setMedia(playlist);
    location = tmpPlayer->metaData(QMediaMetaData::GPSAreaInformation).toString();
    return location;
}


void TheButton::init(TheButtonInfo* i) {
    setIcon( *(i->icon) );
    info =  i;
}


void TheButton::clicked() {
    emit jumpTo(info);
}



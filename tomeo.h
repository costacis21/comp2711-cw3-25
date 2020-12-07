#ifndef TOMEO_H
#define TOMEO_H

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QLabel>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QSlider>
#include <QComboBox>
#include <QTextEdit>
#include <QScrollArea>
#include <QToolBar>
#include <QMediaMetaData>
#include <QFileDialog>
#include "my_videowidget.h"
#include "the_player.h"
#include "the_button.h"
#include "play_pause.h"
#include "forward_button.h"
#include "backward_button.h"
#include "repeat_button.h"
#include "shuffle_button.h"
#include "time_slider.h"

class Tomeo {



public:
    vector<TheButton*> buttons;
    vector<QString> buttonNames;
    vector<QString> buttonLocation;
    QString currentlyPlayingName;
    int currentlyPlayingIndex;
    QString currentlyPlayingLocation;
    QWidget *buttonsWidget;
    QVBoxLayout *buttonsLayout;
    QString searchParam;
    QString playbackComboBoxName;
    bool volumeVisible;
    QPushButton *volumeButton;
    QLabel *title;
    vector<TheButtonInfo> videos;
    MyVideoWidget *videoWidget;
    ThePlayer *player;
    QWidget *window;
    QGridLayout *top;
    QWidget *controlsWidget_1;
    QHBoxLayout *controlsLayout_1;
    QWidget *controlsWidget_2;
    QHBoxLayout *controlsLayout_2;
    QWidget *searchWidgets;
    QScrollArea *scrollArea;
    QWidget *infoWidgets ;
    QWidget *menuWidget;
    QToolBar *toolBar;
    QSlider *volumeSlider;
    QComboBox *playbackComboBox;
    PlaypauseButton *playpause;
    TimeSlider *timeSlider;
    QLabel *currTimeLabel;
    QPushButton *fullscreenButton;
    int argc;
    QApplication *app;
    char *argv[];

    Tomeo(QApplication *myApp,int myArgc, char* myArgv[]){
        app=myApp;
        argc=myArgc;
        for(int i=0;i<argc;i++)
            argv[i]=myArgv[i];


    };
    void createButtons();
    void removeButtons();


    void createControls_1();
    void createConnections_controls_1();

    void createControls_2();
    void createInfos();
    void createScroll();
    void createSearch();
    void createToolBar();
    vector<TheButtonInfo> getInfoIn (string loc);
    QString getNameFromURL(QUrl url);

    int show();



};


#endif // TOMEO_H

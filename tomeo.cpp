/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

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
#include <QToolBar>

#include "the_player.h"
#include "the_button.h"
#include "play_pause.h"
#include "forward_button.h"
#include "backward_button.h"
#include "repeat_button.h"
#include "shuffle_button.h"
#include "time_slider.h"


using namespace std;
bool volumeVisible;
QSlider *volumeSlider;
QComboBox *playbackComboBox;
ThePlayer *player;
playpauseButton *playpause;
ForwardButton *forwardButton;
BackwardButton *backwardButton;
RepeatButton *repeatOn;
ShuffleButton *shuffleOn;
TimeSlider *timeSlider;
QLabel *currTimeLabel ;
QAction *help;
QMessageBox *helpBox;
QGridLayout *top;
QVideoWidget *videoWidget;
QWidget *controlsWidget_1;
QWidget *controlsWidget_2;
QLabel *buttonInstructions;
QWidget *buttonWidget;
QToolBar *toolBar;
QWidget *menuWidget;
QPushButton *volumeButton;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


void createConnections(){
    volumeVisible=false;
    //show volume controll connection

    QObject::connect(volumeButton, &QPushButton::clicked,
                    [=]() { volumeVisible=!volumeVisible;volumeSlider->setVisible(volumeVisible); }
                );
    //volume and speed connections
        QObject::connect(volumeSlider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
        QObject::connect(playbackComboBox, QOverload<int>::of(&QComboBox::activated),
            [=](int index){ if(!index){player->setPlaybackRate(1);}else{player->setPlaybackRate(0.5* index); }});


    //playpause button connection
        QObject::connect(playpause, SIGNAL(clicked()), player, SLOT(changePlayPause()));

    //repeat shuffle connection
        QObject::connect(repeatOn, SIGNAL(clicked()), player, SLOT(changeRepeat()));

    //time position of video connections
        QObject::connect(player, SIGNAL( positionChanged(qint64)), timeSlider, SLOT(setValueqint(qint64)));
        QObject::connect(timeSlider, SIGNAL(valueChanged(int)), player, SLOT(seek(int)));
        QObject::connect(player, SIGNAL(timeS(int,int)), timeSlider, SLOT(setRange(int,int)));//changes range of slider to accomodate new video

    //next previous button connections
        QObject::connect(forwardButton, SIGNAL(clicked()), player, SLOT(forward()));
        QObject::connect(backwardButton, SIGNAL(clicked()), player, SLOT(backwards()));
    //info label connections
        QObject::connect(player, SIGNAL(timeduration(QString)), currTimeLabel, SLOT(setText(QString)));

    //toolbar connection
        QObject::connect(help, SIGNAL(hovered()), helpBox,SLOT(exec()));

}

void createControls(){
    //first line of controls

        controlsWidget_1 = new QWidget();
        QHBoxLayout *controlsLayout_1 = new QHBoxLayout();



        controlsWidget_1->setLayout(controlsLayout_1);
        volumeButton = new QPushButton();
        volumeButton->setIcon(QIcon(":/icons/volume.png"));
        volumeButton->setIconSize(QSize(30,30));



        currTimeLabel = new QLabel(controlsWidget_1);
        currTimeLabel->setStyleSheet("font:15pt;");
        playpause = new playpauseButton(controlsWidget_1);
        timeSlider = new TimeSlider(controlsWidget_1);
        timeSlider->setOrientation(Qt::Horizontal);



        volumeSlider = new QSlider(Qt::Vertical);
        volumeSlider->setParent(controlsWidget_1);
        volumeSlider->setMaximum(100);
        volumeSlider->setMinimum(0);
        volumeSlider->setSizePolicy(QSizePolicy ::Minimum , QSizePolicy ::Minimum);

        volumeSlider->setVisible(false);

        playbackComboBox = new QComboBox(controlsWidget_1);
        playbackComboBox->setStyleSheet("font:15pt");
        playbackComboBox->addItem("Playback Speed");
        playbackComboBox->addItem("x0.5");
        playbackComboBox->addItem("x1.0");
        playbackComboBox->addItem("x1.5");
        playbackComboBox->addItem("x2.0");



        controlsLayout_1->addWidget(playpause);
        controlsLayout_1->addWidget(volumeButton);
        controlsLayout_1->addWidget(volumeSlider);
        controlsLayout_1->addWidget(timeSlider);
        controlsLayout_1->addWidget(currTimeLabel);
        controlsLayout_1->addWidget(playbackComboBox);





    //second line of controls
        controlsWidget_2 = new QWidget();
        QHBoxLayout *controlsLayout_2 = new QHBoxLayout();
        controlsLayout_2->setSpacing(8);

        controlsWidget_2->setLayout(controlsLayout_2);

        forwardButton = new ForwardButton(controlsWidget_2);
        backwardButton = new BackwardButton(controlsWidget_2);
        repeatOn = new RepeatButton(controlsWidget_2);
        shuffleOn= new ShuffleButton(controlsWidget_2);

        controlsLayout_2->addWidget(repeatOn,0);
        controlsLayout_2->addWidget(shuffleOn,0);
        controlsLayout_2->addWidget(backwardButton,0);
        controlsLayout_2->addWidget(forwardButton,0);
        controlsLayout_2->addWidget(repeatOn,0);
}


void addToTop(){

    // add video, the buttons and controls to the top level widget
    top->addWidget(videoWidget,1,0,10,1);
    top->addWidget(controlsWidget_1,11,0,1,1);
    top->addWidget(controlsWidget_2,12,0,1,1);

    top->addWidget(buttonInstructions,13,0,1,1);

    top->addWidget(buttonWidget,14,0,1,1);
    top->addWidget(toolBar,0,0);

}

void createToolBox(){
    menuWidget = new QWidget();

    toolBar = new QToolBar(menuWidget);
    help = new QAction();
    help->setText("Help");
    helpBox = new QMessageBox();
    helpBox->setText("For help please visit our site\n\nwww.tomeo.doesnotexist.com");
    helpBox->setStandardButtons(QMessageBox::Ok);
    helpBox->setWindowTitle("Help");
    toolBar->addAction(help);
}

int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {

        QMessageBox *alert;
        alert->setText("No videos in arguments");
        alert->exec();
        exit(-1);
    }

    // the widget that will show the video
    videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a row of buttons
    buttonWidget = new QWidget();
    // a list of the buttons
    vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);


    // create the four buttons
    for ( int i = 0; i < 4; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the main window and layout
    QWidget window;
    window.setStyleSheet("background-color:#FFFFFF;");

    top = new QGridLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);



    buttonInstructions = new QLabel("Select a video to play");
    buttonInstructions->setStyleSheet("font:18pt;font-weight: bold;");
    createToolBox();
    createControls();
    addToTop();
    createConnections();





    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}

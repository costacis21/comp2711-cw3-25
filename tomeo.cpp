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
#include "the_player.h"
#include "the_button.h"
#include "play_pause.h"
#include "forward_button.h"
#include "backward_button.h"
#include "repeat_button.h"
#include "shuffle_button.h"
#include "time_slider.h"


using namespace std;

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

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
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
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

//first line of controls

    QWidget *controlsWidget_1 = new QWidget();
    QHBoxLayout *controlsLayout_1 = new QHBoxLayout();



    controlsWidget_1->setLayout(controlsLayout_1);


    QLabel *currTimeLabel = new QLabel(controlsWidget_1);
    playpauseButton *playpause = new playpauseButton(controlsWidget_1);
    TimeSlider *timeSlider = new TimeSlider(controlsWidget_1);
    timeSlider->setOrientation(Qt::Horizontal);



    QSlider *volumeSlider = new QSlider(Qt::Vertical);
    volumeSlider->setParent(controlsWidget_1);
    volumeSlider->setMaximum(100);
    volumeSlider->setMinimum(0);

    QComboBox *settingsComboBox = new QComboBox(controlsWidget_1);
    settingsComboBox->addItem("Settings");
    settingsComboBox->setItemIcon(0,QIcon(":/icons/gear_settings.png"));

    settingsComboBox->addItem("Playback Speed");
    settingsComboBox->addItem("FullScreen");
    settingsComboBox->addItem("Quality");



    controlsLayout_1->addWidget(playpause);
    controlsLayout_1->addWidget(volumeSlider);
    controlsLayout_1->addWidget(timeSlider);
    controlsLayout_1->addWidget(currTimeLabel);
    controlsLayout_1->addWidget(settingsComboBox);





//second line of controls
    QWidget *controlsWidget_2 = new QWidget();
    QHBoxLayout *controlsLayout_2 = new QHBoxLayout();
    controlsLayout_2->setSpacing(8);

    controlsWidget_2->setLayout(controlsLayout_2);

    ForwardButton *forwardButton = new ForwardButton(controlsWidget_2);
    BackwardButton *backwardButton = new BackwardButton(controlsWidget_2);
    RepeatButton *repeatOn = new RepeatButton(controlsWidget_2);
    ShuffleButton *shuffleOn= new ShuffleButton(controlsWidget_2);


    QSlider *speedControl = new QSlider(Qt::Horizontal);
    speedControl->setParent(controlsWidget_2);
    speedControl->setMaximum(10);
    speedControl->setMinimum(1);
    speedControl->setValue(5);
    speedControl->setTickInterval(1);


//    controlsLayout_2->addWidget(speedControl);
    controlsLayout_2->addWidget(repeatOn,10);
    controlsLayout_2->addWidget(shuffleOn,10);
    controlsLayout_2->addWidget(backwardButton,10);
    controlsLayout_2->addWidget(forwardButton,10);
    controlsLayout_2->addWidget(repeatOn,10);






//third line of controls
    QWidget *infoWidgets = new QWidget();
    QHBoxLayout *infoLayout = new QHBoxLayout();

    infoWidgets->setLayout(infoLayout);

    QLabel *title = new QLabel(infoWidgets);
    infoLayout->addWidget(title);
    QLabel *time = new QLabel(infoWidgets);
    infoLayout->addWidget(time);



    // add video, the buttons and controls to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(controlsWidget_1);
    top->addWidget(controlsWidget_2);

    top->addWidget(buttonWidget);
    top->addWidget(infoWidgets);







//volume and speed connections
    QObject::connect(volumeSlider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
    QObject::connect(speedControl, SIGNAL(valueChanged(int)), player, SLOT(setSpeed(int)));

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
    QObject::connect(player, SIGNAL(namechange(QString)), title, SLOT(setText(QString)));
    QObject::connect(player, SIGNAL(timeduration(QString)), currTimeLabel, SLOT(setText(QString)));




    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}

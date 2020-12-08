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

#include "tomeo.h"




using namespace std;

QString Tomeo::getNameFromURL(QUrl url){
    QString currUrl= url.toString();
    QStringList pieces = currUrl.split( "/" );
    QString neededWord = pieces.value( pieces.length() - 1 );
    pieces = neededWord.split( "." );
    neededWord = pieces.value( pieces.length()-2 );
    return neededWord;
}

void Tomeo::removeButtons(){

    for(int i=0;i<buttonsLayout->count();i++){
        if(searchParam.isEmpty()){
            buttonsLayout->itemAt(i)->widget()->setVisible(true);
            continue;
        }
        QString name;
        name = getNameFromURL(*buttons.at(i)->info->url);
        cout << name.toStdString()<<searchParam.toStdString()<< endl;

        if(searchParam=="Austria"& name =="cycling"){
            buttonsLayout->itemAt(i)->widget()->setVisible(true);
        }else if(searchParam!=name){
            buttonsLayout->itemAt(i)->widget()->setVisible(false);
        }else{
            buttonsLayout->itemAt(i)->widget()->setVisible(true);
        }

}
}

// read in videos and thumbnails to this directory
vector<TheButtonInfo> Tomeo::getInfoIn (string loc) {

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
                        QString currUrl= url->toString();
                        QStringList pieces = currUrl.split( "/" );
                        QString neededWord = pieces.value( pieces.length() - 1 );
                        pieces = neededWord.split( "." );
                        neededWord = pieces.value( pieces.length()-2 );
                        buttonNames.push_back(neededWord);

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





void Tomeo::createButtons(){
    // create the four buttons
    buttonsLayout = new QVBoxLayout();
    buttonsWidget = new QWidget();
    buttonsWidget->setLayout(buttonsLayout);

    for ( int i = 0; i < videos.size()-1; i++ ) {
        // a row of buttons
        QWidget *buttonRowWidget = new QWidget(buttonsWidget);
        QGridLayout *buttonRowLayout = new QGridLayout();
        buttonRowWidget->setLayout(buttonRowLayout);

        TheButton *button = new TheButton(buttonRowWidget);
        QLabel *buttonLabel_title = new QLabel(buttonRowWidget);
        QLabel *buttonLabel_description = new QLabel(buttonRowWidget);
        QLabel *buttonLabel_duration = new QLabel(buttonRowWidget);
        buttonLabel_title->setStyleSheet("font: 14pt;font-weight:bold;");
        buttonLabel_description->setStyleSheet("font: 12pt;font-weight:100;");
        buttonLabel_description->setWordWrap(true);
        buttonLabel_duration->setStyleSheet("font: 12pt;font-weight:100;");



        QComboBox *queueCombo = new QComboBox(/*buttonRowWidget*/);
        queueCombo->addItem("...");
        queueCombo->addItem("Add to queue");
        queueCombo->setFixedSize(QSize(40,30));
        queueCombo->setStyleSheet("background-color: #e0e0e0;font:15pt;selection-color:black;");

        QObject::connect(queueCombo, QOverload<int>::of(&QComboBox::activated),
            [=](){ queueCombo->setCurrentIndex(0);});




        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* )));
        // when clicked, tell the player to play.
        buttons.push_back(button);
        if(buttonNames.at(i)=="cycling"){
            buttonLabel_title->setText(buttonNames.at(i));
            buttonLabel_description->setText("Video cycling in Austria");
            buttonLabel_duration->setText("0:18");

        }
        else{
            buttonLabel_title->setText(buttonNames.at(i));
            buttonLabel_description->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit,\
                                             sed do eiusmod tempor incididunt ut labore");
            buttonLabel_duration->setText("0:10");
        }
        buttonRowLayout->addWidget(button,0,0,5,5);
        buttonRowLayout->addWidget(buttonLabel_title,0,5,1,1);
        buttonRowLayout->addWidget(buttonLabel_description,1,5,2,1);
        buttonRowLayout->addWidget(buttonLabel_duration,3,5,1,1);


        buttonRowWidget->setFixedWidth(420);
        buttonsLayout->addWidget(buttonRowWidget);

        button->init(&videos.at(i));

    }

}



void Tomeo::createConnections_controls_1(){

    QObject::connect(volumeButton, &QPushButton::clicked,
                    [=]() { volumeVisible=!volumeVisible;volumeSlider->setVisible(volumeVisible); }
                );

//volume and speed connections
    QObject::connect(volumeSlider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
    QObject::connect(playbackComboBox, QOverload<int>::of(&QComboBox::activated),
        [=](int index){ if(!index){player->setPlaybackRate(1);}else{player->setPlaybackRate(0.5* index); }});


//playpause button connection
    QObject::connect(playpause, SIGNAL(clicked()), player, SLOT(changePlayPause()));
    QObject::connect(playpause, SIGNAL(changeplayStatus()),player,SLOT(changePlayPause()));
    QObject::connect(videoWidget,SIGNAL(pauseChanged()),player, SLOT(changePlayPause()));
    QObject::connect(videoWidget,SIGNAL(pauseChanged()),playpause, SLOT(playpauseclicked()));


//time position of video connections
    QObject::connect(player, SIGNAL( positionChanged(qint64)), timeSlider, SLOT(setValueqint(qint64)));
    QObject::connect(timeSlider, SIGNAL(valueChanged(int)), player, SLOT(seek(int)));
    QObject::connect(player, SIGNAL(timeduration(QString)), currTimeLabel, SLOT(setText(QString)));


    QObject::connect(player, SIGNAL(timeS(int,int)), timeSlider, SLOT(setRange(int,int)));
//changes range of slider to accomodate new video

    QObject::connect(fullscreenButton,&QPushButton::clicked,[&]{
        videoWidget->setFullScreen(true);
    });
}



void Tomeo::createControls_1(){
    //first line of controls
    volumeVisible=false;

    controlsWidget_1 = new QWidget();
    controlsLayout_1 = new QHBoxLayout();



    controlsWidget_1->setLayout(controlsLayout_1);

    volumeButton = new QPushButton();
    volumeButton->setFlat(true);
    volumeButton->setIcon(QIcon(":/icons/volume.png"));
    volumeButton->setIconSize(QSize(30,30));

    currTimeLabel = new QLabel(controlsWidget_1);
    currTimeLabel->setStyleSheet("font:15pt;");

    playpause = new PlaypauseButton(controlsWidget_1);
    timeSlider = new TimeSlider(controlsWidget_1);
    timeSlider->setOrientation(Qt::Horizontal);



    volumeSlider = new QSlider(Qt::Vertical);
    volumeSlider->setParent(controlsWidget_1);
    volumeSlider->setMaximum(100);
    volumeSlider->setMinimum(0);
    volumeSlider->setSizePolicy(QSizePolicy ::Minimum , QSizePolicy ::Minimum);
    volumeSlider->setVisible(false);

    playbackComboBox = new QComboBox(controlsWidget_1);
    playbackComboBox->setStyleSheet("font:15pt; selection-color:black;");

    playbackComboBox->addItem("Playback Speed");
    playbackComboBox->addItem("x0.5");
    playbackComboBox->addItem("x1.0");
    playbackComboBox->addItem("x1.5");
    playbackComboBox->addItem("x2.0");

    fullscreenButton = new QPushButton(controlsWidget_1);
    fullscreenButton->setIcon(QIcon(":/icons/fullscreen.png"));
    fullscreenButton->setFlat(true);
    fullscreenButton->setIconSize(QSize(40,40));
    fullscreenButton->setFocusPolicy(Qt::NoFocus);



    controlsLayout_1->addWidget(playpause);
    controlsLayout_1->addWidget(volumeButton);
    controlsLayout_1->addWidget(volumeSlider);
    controlsLayout_1->addWidget(timeSlider);
    controlsLayout_1->addWidget(currTimeLabel);
    controlsLayout_1->addWidget(playbackComboBox);
    controlsLayout_1->addWidget(fullscreenButton);

    createConnections_controls_1();
}






void Tomeo::createControls_2(){//second line of controls
    controlsWidget_2 = new QWidget();
    controlsLayout_2 = new QHBoxLayout();

    controlsWidget_2->setLayout(controlsLayout_2);

    ForwardButton *forwardButton = new ForwardButton(controlsWidget_2);
    BackwardButton *backwardButton = new BackwardButton(controlsWidget_2);
    RepeatButton *repeatOn = new RepeatButton(controlsWidget_2);
    ShuffleButton *shuffleOn= new ShuffleButton(controlsWidget_2);


    controlsLayout_2->addWidget(repeatOn,10);
    controlsLayout_2->addWidget(shuffleOn,10);
    controlsLayout_2->addWidget(backwardButton,10);
    controlsLayout_2->addWidget(forwardButton,10);
    controlsLayout_2->addWidget(repeatOn,10);

    //repeat shuffle connection
        QObject::connect(repeatOn, SIGNAL(clicked()), player, SLOT(changeRepeat()));
    //next previous button connections
        QObject::connect(forwardButton, SIGNAL(clicked()), player, SLOT(forward()));
        QObject::connect(backwardButton, SIGNAL(clicked()), player, SLOT(backwards()));
}





void Tomeo::createInfos(){//third line of controls
    infoWidgets = new QWidget();
    QVBoxLayout *infoLayout = new QVBoxLayout();

    infoWidgets->setLayout(infoLayout);

    QLabel *description = new QLabel(infoWidgets);
    QObject::connect(player,&ThePlayer::mediaStatusChanged,[=](){
        if (currentlyPlayingName=="cycling"){
            description->setText("Austria\nThis is a video from austria while cycling in the mountains");
        }else{
            description->setText("China\nThis is a video with sound which was taken on a GOPRO");

        }

    });
    title = new QLabel(infoWidgets);
    title->setStyleSheet("font:20pt;font-weight:bold;");
    description->setStyleSheet("font:15pt;");
    infoLayout->addWidget(title);
    infoLayout->addWidget(description);

    //info label connections
        QObject::connect(player, &ThePlayer::mediaStatusChanged,
            [=](){title->setText(getNameFromURL(player->currentMedia().canonicalUrl()));});
        QObject::connect(player, &ThePlayer::mediaStatusChanged,
            [=](){currentlyPlayingName=getNameFromURL(player->currentMedia().canonicalUrl());});

}




void Tomeo::createSearch(){
    searchWidgets = new QWidget();
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchWidgets->setLayout(searchLayout);

    QTextEdit *searchBox = new QTextEdit(searchWidgets);

    QPushButton *searchButton = new QPushButton(searchWidgets);

    QComboBox *sortingCombobox = new QComboBox(searchWidgets);
    sortingCombobox->addItem("Sort");
    sortingCombobox->addItem("Sort by date");
    sortingCombobox->addItem("Sort by filename");
    sortingCombobox->addItem("Sort by duration");
    sortingCombobox->setStyleSheet("font:11pt; selection-color:black;");
    searchBox->setFixedHeight(25);
    searchBox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    searchBox->setPlaceholderText("Search by name, location");
    searchButton->setIcon(QIcon(":/icons/search.png"));
    searchWidgets->setSizePolicy(QSizePolicy ::Minimum , QSizePolicy ::Minimum);
    searchBox->setSizePolicy(QSizePolicy ::Minimum , QSizePolicy ::Minimum);


    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(searchBox);
    searchLayout->addWidget(sortingCombobox);

    //search connections
        QObject::connect(searchButton, &QPushButton::clicked, [=]{removeButtons();});
        QObject::connect(searchBox, &QTextEdit::textChanged, [&] {
           searchParam=searchBox->toPlainText();
        });
}

void Tomeo::createScroll(){
    scrollArea = new QScrollArea();


    buttonsLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(buttonsWidget);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollArea->setSizePolicy(QSizePolicy ::Minimum , QSizePolicy ::Minimum);
}




void Tomeo::createToolBar(){
    menuWidget = new QWidget();
    navigationWidget = new QWidget();


    toolBar = new QToolBar(menuWidget);
    navigationBar = new QToolBar(navigationWidget);


    QMessageBox *helpBox = new QMessageBox();
    helpBox->setText("For help please visit our site\n\nwww.tomeo.doesnotexist.tom");
    helpBox->setStandardButtons(QMessageBox::Ok);
    helpBox->setWindowTitle("Help");

    QAction *help = toolBar->addAction("Help");
    QAction *import = toolBar->addAction("Import");

    QAction *goToLibraries = navigationBar->addAction("Libraries");
    QAction *goToTomeo = navigationBar->addAction("Tomeo");

    navigationBar->actions().at(1)->setVisible(false);

    QObject::connect(help, SIGNAL(triggered()), helpBox,SLOT(exec()));
    QObject::connect(goToLibraries, &QAction::triggered,[=](){hideTomeo();showLibraries();});
    QObject::connect(goToTomeo, &QAction::triggered,[=](){hideLibraries();showTomeo();});


}



void Tomeo::showLibraries(){

    if(libraries.size()>0)
        for(int i=0;i<libraries.size()-1;i++)
            libraries.at(i)->show();


    QObject::connect(addLibrary,&QPushButton::clicked,[=](){
        Library *newLib = new Library(librariesWidget);
        libraries.push_back(newLib);

        top->addWidget(newLib,1,5*libraries.size(),5,5);
    });


}

void Tomeo::hideLibraries(){
    if(libraries.size()>0)
        for(int i=0;i<libraries.size()-1;i++)
            libraries.at(i)->hide();
}



void Tomeo::showTomeo(){
    videoWidget->show();

    controlsWidget_1->show();

    searchWidgets->show();

    controlsWidget_2->show();

    scrollArea->show();
    infoWidgets->show();
    navigationBar->actions().at(0)->setVisible(true);
    toolBar->actions().at(1)->setVisible(true);
    toolBar->show();

    navigationBar->actions().at(1)->setVisible(false);

}

void Tomeo::hideTomeo(){
    top->removeWidget(videoWidget);
    top->removeWidget(controlsWidget_1);
    top->removeWidget(searchWidgets);
    top->removeWidget(controlsWidget_2);
    top->removeWidget(scrollArea);
    top->removeWidget(infoWidgets);
    videoWidget->hide();

    controlsWidget_1->hide();

    searchWidgets->hide();

    controlsWidget_2->hide();

    scrollArea->hide();
    infoWidgets->hide();
    toolBar->actions().at(1)->setVisible(false);
    navigationBar->actions().at(0)->setVisible(false);
    navigationBar->actions().at(1)->setVisible(true);


}





int Tomeo::show(){
    // let's just check that Qt is operational first
//    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application

    // collect all the videos in the folder


    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {

        QMessageBox *alert = new QMessageBox();
        alert->setText("No videos in arguments");
            alert->exec();
        exit(-1);
    }

    // the widget that will show the video
    videoWidget = new MyVideoWidget();

    // the QMediaPlayer which controls the playback
    player = new ThePlayer;
    createButtons();

    player->setVideoOutput(videoWidget);




    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the main window and layout
    window= new QWidget();
    window->setStyleSheet("background-color:#FFFFFF;");

    top = new QGridLayout();
    window->setLayout(top);
    window->setWindowTitle("tomeo");
    window->setMinimumSize(900, 680);

    createControls_1();
    createControls_2();
    createInfos();
    createScroll();
    createSearch();
    createToolBar();

    addLibrary = new QPushButton();
    addLibrary->setText("+");
    addLibrary->setStyleSheet("font:100pt;");
    addLibrary->setFixedSize(100,100);
    top->addWidget(addLibrary,0,0,5,5);

    // add video, the buttons and controls to the top level widget
    top->addWidget(videoWidget,1,0,6,6);

    top->addWidget(controlsWidget_1,7,0,1,6);

    top->addWidget(searchWidgets,1,6,1,2);

    top->addWidget(controlsWidget_2,7,6,1,2);

    top->addWidget(scrollArea,2,6,5,2);
    top->addWidget(infoWidgets,8,0,1,1);

    top->addWidget(toolBar,0,0,1,3);
    top->addWidget(navigationWidget,0,7,1,3);




    // showtime!
    window->show();

    return app->exec();

}



int main(int argc, char *argv[]) {
    QApplication app(argc,argv);

    Tomeo *tomeo = new Tomeo(&app,argc,argv);

    // wait for the app to terminate
    return tomeo->show();
}



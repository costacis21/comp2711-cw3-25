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
    QString currUrl = url.toString();
    QStringList pieces = currUrl.split("/");
    QString neededWord = pieces.value(pieces.length() - 1);
    pieces = neededWord.split(".");
    neededWord = pieces.value(pieces.length() - 2);
    return neededWord;
}

void Tomeo::removeButtons(){

    for (int i = 0; i < buttonsLayout->count(); i++) {
        if (searchParam.isEmpty()) {
            buttonsLayout->itemAt(i)->widget()->setVisible(true);
            continue;
        }
        QString name;
        name = getNameFromURL(*buttons.at(i)->info->url);
        cout << name.toStdString() << searchParam.toStdString() << endl;

        if (searchParam == "Austria" & name == "cycling") {
            buttonsLayout->itemAt(i)->widget()->setVisible(true);
        }
        else if (searchParam != name) {
            buttonsLayout->itemAt(i)->widget()->setVisible(false);
        }
        else {
            buttonsLayout->itemAt(i)->widget()->setVisible(true);
        }
    }
}

// read in videos and thumbnails to this directory
vector<TheButtonInfo> Tomeo::getInfoIn(QString loc){

    vector<TheButtonInfo> out = vector<TheButtonInfo>();
    QDir dir(loc);
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

        if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv")) { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV")) { // mac/linux
#endif

                QString thumb = f.left(f.length() - 4) + ".png";
                if (QFile(thumb).exists()) { // if a png thumbnail exists
                    QImageReader* imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile(f)); // convert the file location to a generic url
                        out.push_back(TheButtonInfo(url, ico)); // add to the output list
                        QString currUrl = url->toString();
                        QStringList pieces = currUrl.split("/");
                        QString neededWord = pieces.value(pieces.length() - 1);
                        pieces = neededWord.split(".");
                        neededWord = pieces.value(pieces.length() - 2);
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

    for (int i = 0; i < videos.size() - 1; i++) {
        // a row of buttons
        QWidget* buttonRowWidget = new QWidget(buttonsWidget);
        QGridLayout* buttonRowLayout = new QGridLayout();
        buttonRowWidget->setLayout(buttonRowLayout);

        TheButton* button = new TheButton(buttonRowWidget);
        QLabel* buttonLabel_title = new QLabel(buttonRowWidget);
        QLabel* buttonLabel_description = new QLabel(buttonRowWidget);
        QLabel* buttonLabel_duration = new QLabel(buttonRowWidget);

        buttonLabel_title->setStyleSheet("font: 14pt;font-weight:bold;");
        buttonLabel_description->setStyleSheet("font: 12pt;font-weight:100;");
        buttonLabel_description->setWordWrap(true);
        buttonLabel_duration->setStyleSheet("font: 12pt;font-weight:100;");

        QComboBox* addToLibCombo = new QComboBox(buttonRowWidget);
        addToLibCombo->addItem(" + ");
        addToLibCombo->setFixedSize(QSize(50, 50));
        addToLibCombo->setCursor(QCursor(Qt::PointingHandCursor));
        addToLibCombo->setLayoutDirection(Qt::RightToLeft);

        addToLibCombo->setStyleSheet("QComboBox{font: 13pt; border:0px;text-align: center;color: black; background-color:white}\
                                        QComboBox::drop-down {border-width: 0px;width:0px;}\
                                        QComboBox::down-arrow {image: url(noimg); border-width: 0px;width:0px;}\
                                        QComboBox QAbstractItemView {min-width: 150px;}\
                                       ");

        addToLibComboS.push_back(addToLibCombo);

        QComboBox* removeFromLibCombo = new QComboBox(buttonRowWidget);
        removeFromLibCombo->addItem(" - ");
        removeFromLibCombo->setFixedSize(QSize(50, 50));
        removeFromLibCombo->setCursor(QCursor(Qt::PointingHandCursor));
        removeFromLibCombo->setLayoutDirection(Qt::RightToLeft);

        removeFromLibCombo->setStyleSheet("QComboBox{font: 13pt; border:0px;text-align: center;color: black; background-color:white}\
                                            QComboBox::drop-down {border-width: 0px;width:0px;}\
                                            QComboBox::down-arrow {image: url(noimg); border-width: 0px;width:0px;}\
                                           QComboBox QAbstractItemView {min-width: 150px;}\
                                           ");

        removeFromLibComboS.push_back(removeFromLibCombo);

        QObject::connect(addToLibCombo, QOverload<int>::of(&QComboBox::activated),
            [=](int index) {
                if (index > 0) {
                    libraries.at(index - 1)->addVid(buttons.at(i)->info);
                    removeFromLibCombo->addItem(libraries.at(index - 1)->name);
                    QObject::connect(libraries.at(index - 1)->nameTextEdit, &QTextEdit::textChanged, [=]() {
                        removeFromLibCombo->setItemText(removeFromLibCombo->count() - 1,
                            libraries.at(index - 1)->nameTextEdit->toPlainText());
                    });
                }
                addToLibCombo->setCurrentIndex(0);

            });

        QObject::connect(removeFromLibCombo, QOverload<int>::of(&QComboBox::activated),
            [=](int index) {
                if (index > 0) {
                    for (int j = 0; j < removeFromLibCombo->count(); j++)
                        for (int k = 0; k < libraries.size(); k++)
                            if (removeFromLibCombo->itemText(j) == libraries.at(k)->name) {
                                libraries.at(k)->removeVid(buttons.at(i)->info);
                                removeFromLibCombo->removeItem(index);
                            }
                }
                removeFromLibCombo->setCurrentIndex(0);

            });

        button->connect(button, SIGNAL(jumpTo(TheButtonInfo*)), player, SLOT(jumpTo(TheButtonInfo*)));

        // when clicked, tell the player to play.
        buttons.push_back(button);
        if (buttonNames.at(i) == "cycling") {
            buttonLabel_title->setText(buttonNames.at(i));
            buttonLabel_description->setText("Video cycling in Austria");
            buttonLabel_duration->setText("0:18");
        }
        else {
            buttonLabel_title->setText(buttonNames.at(i));
            buttonLabel_description->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit,\
                                             sed do eiusmod tempor incididunt ut labore");
            buttonLabel_duration->setText("0:10");
        }
        buttonRowLayout->addWidget(button, 0, 0, 5, 5);
        buttonRowLayout->addWidget(buttonLabel_title, 0, 5, 1, 1);
        buttonRowLayout->addWidget(buttonLabel_description, 1, 5, 2, 1);
        buttonRowLayout->addWidget(buttonLabel_duration, 3, 5, 1, 1);
        buttonRowLayout->addWidget(addToLibCombo, 2, 6, 1, 3);
        buttonRowLayout->addWidget(removeFromLibCombo, 3, 6, 1, 3);

        buttonRowWidget->setFixedWidth(420);
        buttonsLayout->addWidget(buttonRowWidget);

        button->init(&videos.at(i));
    }
}

void Tomeo::createConnections_controls_1(){

    QObject::connect(volumeButton, &QPushButton::clicked,
        [=]() { volumeVisible=!volumeVisible;volumeSlider->setVisible(volumeVisible); });

    //volume and speed connections
    QObject::connect(volumeSlider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
    QObject::connect(playbackComboBox, QOverload<int>::of(&QComboBox::activated),
        [=](int index) { if(!index){player->setPlaybackRate(1);}else{player->setPlaybackRate(0.5* index); } });

    //playpause button connection
    QObject::connect(playpause, SIGNAL(clicked()), player, SLOT(changePlayPause()));
    QObject::connect(playpause, SIGNAL(changeplayStatus()), player, SLOT(changePlayPause()));
    QObject::connect(videoWidget, SIGNAL(pauseChanged()), player, SLOT(changePlayPause()));
    QObject::connect(videoWidget, SIGNAL(pauseChanged()), playpause, SLOT(playpauseclicked()));

    //time position of video connections
    QObject::connect(player, SIGNAL(positionChanged(qint64)), timeSlider, SLOT(setValueqint(qint64)));
    QObject::connect(timeSlider, SIGNAL(valueChanged(int)), player, SLOT(seek(int)));
    QObject::connect(player, SIGNAL(timeduration(QString)), currTimeLabel, SLOT(setText(QString)));

    QObject::connect(player, SIGNAL(timeS(int, int)), timeSlider, SLOT(setRange(int, int)));
    //changes range of slider to accomodate new video

    QObject::connect(fullscreenButton, &QPushButton::clicked, [&] {
        videoWidget->setFullScreen(true);
    });
}

void Tomeo::createControls_1(){
    //first line of controls
    volumeVisible = false;

    controlsWidget_1 = new QWidget();
    controlsLayout_1 = new QGridLayout();

    controlsWidget_1->setLayout(controlsLayout_1);

    volumeButton = new QPushButton();
    volumeButton->setFlat(true);
    volumeButton->setIcon(QIcon(":/icons/volume.png"));
    volumeButton->setIconSize(QSize(30, 30));
    volumeButton->setCursor(QCursor(Qt::PointingHandCursor));

    currTimeLabel = new QLabel(controlsWidget_1);
    currTimeLabel->setStyleSheet("font:15pt;");

    playpause = new PlaypauseButton(controlsWidget_1);
    timeSlider = new TimeSlider(controlsWidget_1);
    timeSlider->setOrientation(Qt::Horizontal);

    volumeSlider = new QSlider(Qt::Vertical);
    volumeSlider->setParent(controlsWidget_1);
    volumeSlider->setMaximum(100);
    volumeSlider->setMinimum(0);
    volumeSlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
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
    fullscreenButton->setIconSize(QSize(40, 40));
    fullscreenButton->setFocusPolicy(Qt::NoFocus);
    fullscreenButton->setCursor(QCursor(Qt::PointingHandCursor));

    addBookmarkButton = new QPushButton(controlsWidget_1);
    addBookmarkButton->setIcon(QIcon(":/icons/addBookmark.png"));
    addBookmarkButton->setFlat(true);
    addBookmarkButton->setIconSize(QSize(25, 25));
    addBookmarkButton->setFocusPolicy(Qt::NoFocus);
    addBookmarkButton->setCursor(QCursor(Qt::PointingHandCursor));

    QObject::connect(addBookmarkButton, &QPushButton::clicked, [=]() {
        QPushButton* newBookmark = new QPushButton();
        bookmarks.push_back(newBookmark);
        QTextEdit *newBookmarkName = new QTextEdit();
        bookmarkNames.push_back(newBookmarkName);
        newBookmarkName->setFixedSize(60,20);
        newBookmarkName->setStyleSheet("font:10pt;");
        newBookmarkName->setText("Tag "+ QString::number(bookmarkNames.size()));
        newBookmarkName->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        newBookmarkName->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);


        newBookmark->setIcon(QIcon(":/icons/newBookmark.png"));
        newBookmark->setIconSize(QSize(30, 30));
        newBookmark->setFixedSize(25, 25);
        newBookmark->setFlat(true);
        newBookmark->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

        double oldRange = (timeSlider->maximum() - timeSlider->minimum());
        double newRange = 100;
        double newValue = (((timeSlider->value() - timeSlider->minimum()) * newRange) / oldRange);

        controlsLayout_1->addWidget(newBookmark, 1, 29 + newValue, 1, 1);
        controlsLayout_1->addWidget(newBookmarkName, 2, 29 + newValue, 1, 1);

    });
    QObject::connect(player, &ThePlayer::currentMediaChanged, [=]() {
        for (int i = 0; i < bookmarks.size(); i++) {
            bookmarks.at(i)->hide();
            bookmarkNames.at(i)->hide();
        }
    });
    controlsLayout_1->addWidget(playpause, 0, 0, 1, 10);
    controlsLayout_1->addWidget(volumeButton, 0, 9, 1, 10);
    controlsLayout_1->addWidget(volumeSlider, 0, 19, 1, 10);
    controlsLayout_1->addWidget(timeSlider, 0, 29, 1, 100);
    controlsLayout_1->addWidget(addBookmarkButton, 0, 129, 1, 10);
    controlsLayout_1->addWidget(currTimeLabel, 0, 139, 1, 10);
    controlsLayout_1->addWidget(playbackComboBox, 0, 149, 1, 20);
    controlsLayout_1->addWidget(fullscreenButton, 0, 169, 1, 10);

    createConnections_controls_1();
}

void Tomeo::createControls_2(){ //second line of controls
    controlsWidget_2 = new QWidget();
    controlsLayout_2 = new QHBoxLayout();

    controlsWidget_2->setLayout(controlsLayout_2);

    ForwardButton* forwardButton = new ForwardButton(controlsWidget_2);
    BackwardButton* backwardButton = new BackwardButton(controlsWidget_2);
    RepeatButton* repeatOn = new RepeatButton(controlsWidget_2);
    ShuffleButton* shuffleOn = new ShuffleButton(controlsWidget_2);

    controlsLayout_2->addWidget(repeatOn, 10);
    controlsLayout_2->addWidget(shuffleOn, 10);
    controlsLayout_2->addWidget(backwardButton, 10);
    controlsLayout_2->addWidget(forwardButton, 10);
    controlsLayout_2->addWidget(repeatOn, 10);
    //repeat shuffle connection
    QObject::connect(repeatOn, SIGNAL(clicked()), player, SLOT(changeRepeat()));
    //next previous button connections
    QObject::connect(forwardButton, SIGNAL(clicked()), player, SLOT(forward()));
    QObject::connect(backwardButton, SIGNAL(clicked()), player, SLOT(backwards()));
}

void Tomeo::createInfos(){
    QGridLayout* infoLayout = new QGridLayout();

    infoWidgets->setLayout(infoLayout);

    QLabel* description = new QLabel(infoWidgets);
    QObject::connect(player, &ThePlayer::mediaStatusChanged, [=]() {
        if (currentlyPlayingName == "cycling") {
            description->setText("Austria\nThis is a video from austria while cycling in the mountains");
        }
        else {
            description->setText("China\nThis is a video with sound which was taken on a GOPRO");
        }

    });
    QPushButton* editTitleButton = new QPushButton(infoWidgets);
    title = new QTextEdit(infoWidgets);
    editTitleButton->setFixedSize(30, 30);
    editTitleButton->setIcon(QIcon(":/icons/noEditText.png"));

    QObject::connect(editTitleButton, &QPushButton::clicked, [=]() {
        if (title->isReadOnly())
            editTitleButton->setIcon(QIcon(":/icons/noEditText.png"));
        else
            editTitleButton->setIcon(QIcon(":/icons/editText.png"));
        title->setReadOnly(!title->isReadOnly());

    });
    title->setStyleSheet("font:18pt;font-weight:bold;border:0px;");
    title->setFixedHeight(30);
    title->setReadOnly(false);
    title->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    title->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    description->setStyleSheet("font:15pt;");
    infoLayout->addWidget(title, 0, 1, 1, 5);
    infoLayout->addWidget(editTitleButton, 0, 0, 1, 1);
    infoLayout->addWidget(description, 1, 0, 3, 5);

    //info label connections
    QObject::connect(player, &ThePlayer::mediaStatusChanged,
        [=]() { title->setText(getNameFromURL(player->currentMedia().canonicalUrl())); });
    QObject::connect(player, &ThePlayer::mediaStatusChanged,
        [=]() { currentlyPlayingName = getNameFromURL(player->currentMedia().canonicalUrl()); });
}

void Tomeo::createSearch(){
    searchWidgets = new QWidget();
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchWidgets->setLayout(searchLayout);

    QTextEdit* searchBox = new QTextEdit(searchWidgets);

    QPushButton* searchButton = new QPushButton(searchWidgets);
    searchButton->setCursor(QCursor(Qt::PointingHandCursor));

    QComboBox* sortingCombobox = new QComboBox(searchWidgets);
    sortingCombobox->addItem("Sort");
    sortingCombobox->addItem("Sort by date");
    sortingCombobox->addItem("Sort by filename");
    sortingCombobox->addItem("Sort by duration");
    sortingCombobox->setStyleSheet("font:11pt; selection-color:black;");
    searchBox->setFixedHeight(25);
    searchBox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    searchBox->setPlaceholderText("Search by name, location");
    searchButton->setIcon(QIcon(":/icons/search.png"));
    searchWidgets->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    searchBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(searchBox);
    searchLayout->addWidget(sortingCombobox);

    //search connections
    QObject::connect(searchButton, &QPushButton::clicked, [=] {
    QObject::connect(searchBox, &QTextEdit::textChanged, [&] {
        searchParam = searchBox->toPlainText();
    });
     removeButtons();
    });
}

void Tomeo::createScroll(){

    buttonsLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(buttonsWidget);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollArea->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void Tomeo::createToolBar(){
    menuWidget = new QWidget();
    navigationWidget = new QWidget();

    toolBar = new QToolBar(menuWidget);
    navigationBar = new QToolBar(navigationWidget);

    navigationWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    navigationBar->setFixedSize(100, 50);
    toolBar->setFixedHeight(50);

    QMessageBox* helpBox = new QMessageBox();
    helpBox->setText("For help please visit our site\n\nwww.tomeo.doesnotexist.tom");
    helpBox->setStandardButtons(QMessageBox::Ok);
    helpBox->setWindowTitle("Help");

    QAction* help = toolBar->addAction("Help");

    QAction* import = toolBar->addAction("Import");
    QAction* goToLibraries = navigationBar->addAction("Libraries");
    QAction* goToTomeo = navigationBar->addAction("Tomeo");
    navigationBar->setStyleSheet("QToolBar{;background-color:grey;border-radius:30px;}\
                                QToolButton:hover{background-color:#d3d3d3;color:black}\
                                QToolButton{font: 17pt;color:white;background-color:grey;border-radius:15px;}");
    toolBar->setStyleSheet("QToolBar{background-color:grey;border-radius:30px;}\
                             QToolButton:hover{background-color:#d3d3d3;color:black}\
                             QToolButton{font: 17pt;color:white;background-color:grey;border-radius:15px;}");
    navigationBar->actions().at(1)->setVisible(false);

    QObject::connect(help, SIGNAL(triggered()), helpBox, SLOT(exec()));
    QObject::connect(goToLibraries, &QAction::triggered, [=]() {hideTomeo();showLibraries(); });
    QObject::connect(goToTomeo, &QAction::triggered, [=]() {hideLibraries();showTomeo(); });
    QObject::connect(import, &QAction::triggered, [=]() {
        QString path = QFileDialog::getExistingDirectory(Tomeo::window, "Choose Video Folder", ".", QFileDialog::ReadOnly);
        videos = getInfoIn(path);
        top->removeWidget(firstTime);
        firstTime->hide();
        createButtons();
        player->setContent(&buttons, &videos);

        createScroll();
        createInfos();
        connectLibsToButtons();

    });
}

void Tomeo::connectLibsToButtons(){
    for (int j = 0; j < libraries.size(); j++) {
        if (buttons.size() > 0)
            j = addToLibComboS.at(0)->count() - 1;
        Library* newLib = libraries.at(j);

        QObject::connect(newLib->optionsCombo, QOverload<int>::of(&QComboBox::activated),
            [=](int index) {
                if (index == 2)
                    for (int i = 0; i < libraries.size(); i++)
                        if (libraries.at(i) == newLib) {
                            top->removeWidget(libraries.at(i));
                            libraries.at(i)->hide();
                            libraries.erase(libraries.begin() + i);
                        }

            });

        for (int i = 0; i < addToLibComboS.size(); i++) {

            addToLibComboS.at(i)->addItem(newLib->name);

            QObject::connect(newLib->nameTextEdit, &QTextEdit::textChanged, [=]() {
                addToLibComboS.at(i)->setItemText(j + 1,
                    newLib->nameTextEdit->toPlainText());
            });
        }
    }
}

void Tomeo::showLibraries(){
    top->removeWidget(firstTime);
    firstTime->hide();
    addLibrary->show();
    addLibraryInstruction->show();

    for (int i = 0; i < libraries.size(); i++)
        libraries.at(i)->show();
}

void Tomeo::hideLibraries(){

    for (unsigned i = 0; i < libraries.size(); i++) {
        libraries.at(i)->hide();
    }
    addLibraryInstruction->hide();

    addLibrary->hide();
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
    videoWidget->hide();
    controlsWidget_1->hide();
    searchWidgets->hide();
    controlsWidget_2->hide();
    scrollArea->hide();
    infoWidgets->hide();
    videoWidget->hide();

    toolBar->actions().at(1)->setVisible(false);
    navigationBar->actions().at(0)->setVisible(false);
    navigationBar->actions().at(1)->setVisible(true);
}

void Tomeo::createAddLibraryButton(){
    addLibrary = new QPushButton();
    addLibrary->setIcon(QIcon(":/icons/addLibrary.png"));
    addLibrary->setIconSize(QSize(75, 75));
    addLibrary->setFixedSize(100, 100);
    addLibrary->setCursor(QCursor(Qt::PointingHandCursor));

    addLibraryInstruction = new QLabel();
    addLibraryInstruction->setText("Press + to Create Library");
    addLibraryInstruction->setStyleSheet("font: 25pt; font-weight: bold;");

    QObject::connect(addLibrary, &QPushButton::clicked, [=]() {
        Library* newLib = new Library();
        newLib->nameTextEdit->setText("Untitled " + QString::number(libraries.size()));
        libraries.push_back(newLib);
        if (libraries.size() <= 2)
            top->addWidget(newLib, 2, (3 * libraries.size()) - 1, 3, 3);
        else
            top->addWidget(newLib, 3, (3 * libraries.size() - 2 * libraries.size()) - 1, 3, 3);
        connectLibsToButtons();
    });
}

int Tomeo::setUpLayout(){

    createControls_1();
    createControls_2();
    createSearch();
    createToolBar();
    createAddLibraryButton();

    player->setVideoOutput(videoWidget);

    if (libraries.size() > 0)
        for (int i = 0; i < libraries.size() - 1; i++) {
            top->addWidget(libraries.at(i), 1, libraries.size(), 3, 3);
        }
    top->addWidget(addLibraryInstruction, 1, 3, 1, 5);
    addLibraryInstruction->hide();
    top->addWidget(addLibrary, 1, 0, 5, 5);
    addLibrary->hide();

    top->addWidget(videoWidget, 1, 0, 6, 6);

    top->addWidget(controlsWidget_1, 7, 0, 1, 6);

    top->addWidget(searchWidgets, 1, 6, 1, 2);

    top->addWidget(controlsWidget_2, 7, 6, 1, 2);
    top->addWidget(toolBar, 0, 0, 1, 8);
    top->addWidget(navigationBar, 0, 7, 1, 1);
    top->addWidget(videoWidget, 1, 0, 6, 6);
    top->addWidget(firstTime, 1, 0, 6, 6);

    top->addWidget(controlsWidget_1, 7, 0, 1, 6);

    if (argc == 2) {
        videos = getInfoIn(argv[1]);

        // tell the player what buttons and videos are available
        player->setContent(&buttons, &videos);

        createButtons();
        createScroll();
        createInfos();

        // add video, the buttons and controls to the top level widget
    }
    top->addWidget(scrollArea, 2, 6, 5, 2);
    top->addWidget(infoWidgets, 8, 0, 1, 6);
    // showtime!
    window->show();

    return app->exec();
}

int main(int argc, char* argv[]){
    QApplication app(argc, argv);

    Tomeo* tomeo = new Tomeo(&app, argc, argv);

    // wait for the app to terminate
    return tomeo->setUpLayout();
}

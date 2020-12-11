#include "library.h"

void Library::setName(QString newName){
    name = newName;
}

//remove video from library
void Library::removeVid(TheButtonInfo* newVid){
    for(int i=0;i<videoInfos.size();i++)
        if(newVid->url==videoInfos.at(i)->url){
            //remove thumbnail
            layout->removeWidget(thumbnails.at(i));
            thumbnails.at(i)->hide();
            thumbnails.at(i)->clear();

            thumbnails.erase(thumbnails.begin()+i);

            videoInfos.erase(videoInfos.begin()+i);
        }
    if(videoInfos.size()==0){
        emptyLibrary->show();
        layout->addWidget(emptyLibrary,0,0,4,4);
    }
}

//add video to this library
void Library::addVid(TheButtonInfo* newVid){
    emptyLibrary->hide();
    layout->removeWidget(emptyLibrary);
    videoInfos.push_back(newVid);
    QLabel *newThumbnail = new QLabel(this);
    thumbnails.push_back(newThumbnail);
    newThumbnail->setPixmap(newVid->icon->pixmap(newVid->icon->actualSize(QSize(1024, 1024))));

    newThumbnail->setFixedSize(100,100);
    layout->addWidget(newThumbnail,0,3*(videoInfos.size()-1),3,3);
}


//create export window
void Library::exportLibrary(){
    connect(optionsCombo, QOverload<int>::of(&QComboBox::activated),
            [=](int index){
        if(index==1){
            QWidget *exportWidget = new QWidget();
            QGridLayout *exportLayout = new QGridLayout();
            exportWidget->setLayout(exportLayout);
            QPushButton *fbButton = new QPushButton("Share on Facebook",exportWidget);
            QPushButton *igButton = new QPushButton("Share on Instagram",exportWidget);
            QPushButton *twButton = new QPushButton("Share on Twitter",exportWidget);
            QPushButton *lnkdInButton = new QPushButton("Share to LinkedIn",exportWidget);
            QPushButton *utbButton = new QPushButton("Upload to Youtube",exportWidget);
            QPushButton *cancel = new QPushButton("cancel",exportWidget);
            connect(cancel,&QPushButton::clicked,[=](){exportWidget->hide();});

            QLabel *shareInfoLbl = new QLabel(exportWidget);
            shareInfoLbl->setText("Share to Social Media");
            shareInfoLbl->setStyleSheet("font:25pt;text-align:center;font-weight:bold;");
            fbButton->setFixedHeight(50);
            igButton->setFixedHeight(50);
            twButton->setFixedHeight(50);
            lnkdInButton->setFixedHeight(50);
            utbButton->setFixedHeight(50);
            cancel->setFixedHeight(30);

            fbButton->setStyleSheet("font:20pt;background-color: white;");
            igButton->setStyleSheet("font:20pt;background-color: white;");
            twButton->setStyleSheet("font:20pt;background-color: white;");
            lnkdInButton->setStyleSheet("font:20pt;background-color: white;");
            utbButton->setStyleSheet("font:20pt;background-color: white;");
            cancel->setStyleSheet("font:13pt;background-color: white;");
            exportLayout->addWidget(shareInfoLbl,0,0,Qt::AlignHCenter);
            exportLayout->addWidget(fbButton,1,0);
            exportLayout->addWidget(igButton,2,0);
            exportLayout->addWidget(twButton,3,0);
            exportLayout->addWidget(lnkdInButton,4,0);
            exportLayout->addWidget(utbButton,5,0);
            exportLayout->addWidget(cancel,6,0,Qt::AlignRight);

            exportWidget->setWindowTitle("Share");
            exportWidget->setMinimumSize(700, 400);
            exportWidget->show();

        }
        optionsCombo->setCurrentIndex(0);
    });

}

#ifndef LIBRARY_H
#define LIBRARY_H
#include <QWidget>
#include <QFrame>
#include <QGridLayout>
#include <QComboBox>
#include <QTextEdit>
#include "the_button.h"

class Library  : public QWidget {
    Q_OBJECT
public:
    QGridLayout *layout;
    std::vector <TheButtonInfo*> videoInfos;
    QString name;
    QWidget *optionsWidget;
    QTextEdit *nameTextEdit;
    QComboBox *optionsCombo;
    QFrame *thumbnailFrame;

    Library(QWidget *parent) :  QWidget(parent) {
        layout=new QGridLayout();

        setLayout(layout);

        optionsCombo = new QComboBox();
        nameTextEdit = new QTextEdit();

        optionsCombo->addItem("...");
        optionsCombo->addItem("Export");
        optionsCombo->addItem("Delete");
        QPushButton *tmp = new QPushButton();
        thumbnailFrame = new QFrame();

        layout->addWidget(tmp,0,0,5,5);
        layout->addWidget(optionsCombo,5,5,1,5);



        setFixedSize(100,100);

    }


    void addVid(TheButtonInfo* newVid);
    void setName(QString newName);


};


#endif // LIBRARY_H

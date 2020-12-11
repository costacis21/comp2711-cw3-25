#ifndef LIBRARY_H
#define LIBRARY_H
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QPicture>
#include <QMessageBox>
#include "the_button.h"


class Library  : public QWidget {
    Q_OBJECT
public:
    QGridLayout *layout;
    std::vector <TheButtonInfo*> videoInfos;
    std::vector <QLabel*> thumbnails;

    QString name;
    QTextEdit *nameTextEdit;
    QComboBox *optionsCombo;
    QPushButton *editName;
    QLabel *emptyLibrary;

    Library() :  QWidget() {
        layout=new QGridLayout();
        setLayout(layout);
        editName= new QPushButton();
        optionsCombo = new QComboBox();
        nameTextEdit = new QTextEdit();
        emptyLibrary = new QLabel();


        editName->setIcon(QIcon(":/icons/noEditText.png"));
        editName->setIconSize(QSize(25,25));
        editName->setFixedSize(25,25);
        editName->setFlat(true);
        editName->setCursor(QCursor(Qt::PointingHandCursor));

        nameTextEdit->setFixedHeight(25);
        nameTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        optionsCombo->addItem("...");
        optionsCombo->addItem("Export");
        optionsCombo->addItem("Delete");
        optionsCombo->setStyleSheet ("QComboBox{font: 13pt; border:0px;text-align: center;color: black; background-color:white}\
                                       QComboBox::drop-down {border-width: 0px;width:0px;}\
                                       QComboBox::down-arrow {image: url(noimg); border-width: 0px;width:0px;}\
                                     QComboBox QAbstractItemView {min-width: 150px;}\
                                      ");


        emptyLibrary->setText("This Library is Empty");
        emptyLibrary->setStyleSheet("font: 20pt;");

        layout->addWidget(editName,5,0,1,1);
        layout->addWidget(nameTextEdit,5,1,1,4);
        layout->addWidget(optionsCombo,5,5,1,5);
        layout->addWidget(emptyLibrary,0,0,4,4);



       setFixedHeight(450);



        exportLibrary();
        connect(nameTextEdit,&QTextEdit::textChanged,[=](){name=nameTextEdit->toPlainText();});
        connect(editName,&QPushButton::clicked,[=](){
            if(nameTextEdit->isReadOnly())
                editName->setIcon(QIcon(":/icons/noEditText.png"));
            else
                editName->setIcon(QIcon(":/icons/editText.png"));
            nameTextEdit->setReadOnly(!nameTextEdit->isReadOnly());


        });
    }


    void addVid(TheButtonInfo* newVid);
    void removeVid(TheButtonInfo* newVid);
    void exportLibrary();
    void setName(QString newName);


};


#endif // LIBRARY_H

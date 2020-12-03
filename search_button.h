#ifndef SEARCH_BUTTON_H
#define SEARCH_BUTTON_H


#include <QPushButton>
#include <QIcon>



class SearchButton : public QPushButton {
    Q_OBJECT

public:

     SearchButton(QWidget *parent) :  QPushButton(parent) {
         setIcon(QIcon(":/icons/search.png"));
         setIconSize(QSize(20,20));
         setFlat(true);
     }






};



#endif // SEARCH_BUTTON_H

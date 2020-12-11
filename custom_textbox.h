#ifndef CUSTOM_TEXTBOX_H
#define CUSTOM_TEXTBOX_H



#include <QTextEdit>
#include <QEvent>
#include <QHoverEvent>



class CustomTextBox : public QTextEdit {
    Q_OBJECT

public:

    CustomTextBox() :  QTextEdit() {
        setMouseTracking(true);
        setCursor(QCursor(Qt::IBeamCursor));

    }


protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};
#endif // CUSTOM_TEXTBOX_H

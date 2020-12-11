#include "custom_textbox.h"





void CustomTextBox::mouseDoubleClickEvent(QMouseEvent *event)
{
        setReadOnly(!isReadOnly());
        event->accept();

}

void CustomTextBox::mousePressEvent(QMouseEvent *event)
{
    QTextEdit::mousePressEvent(event);
}

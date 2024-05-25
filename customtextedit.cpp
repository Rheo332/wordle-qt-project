#include "customtextedit.h"

CustomTextEdit::CustomTextEdit(QWidget *parent)
    : QTextEdit(parent)
{}

void CustomTextEdit::paintEvent(QPaintEvent *event)
{
    int originalCursorWidth = cursorWidth();
    setCursorWidth(0);

    QTextEdit::paintEvent(event);

    setCursorWidth(originalCursorWidth);
}

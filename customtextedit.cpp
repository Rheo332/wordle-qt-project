#include "customtextedit.h"
#include <QDebug>

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

void CustomTextEdit::keyPressEvent(QKeyEvent *event)
{
    emit keyPress(event);

    // override the default behavior by not calling the base event
    // QTextEdit::keyPressEvent(event);
}

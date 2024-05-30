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
    emit keyPressSignal(event);
}

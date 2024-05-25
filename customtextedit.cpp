#include "customtextedit.h"
#include <QDebug>
#include <QKeyEvent>

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
    if (event->key() == Qt::Key_Backspace) {
        emit textChanged();
        qDebug() << "backspace";
    }
    qDebug() << event->key();
    QTextEdit::keyPressEvent(event);
}

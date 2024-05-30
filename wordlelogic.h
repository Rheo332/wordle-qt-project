#ifndef WORDLELOGIC_H
#define WORDLELOGIC_H

#include <QDebug>
#include <QFile>
#include <QList>
#include <QTextStream>
#include <customtextedit.h>
#include <filelogic.h>

class WordleLogic
{
public:
    QList<CustomTextEdit *> allTextEdits;
    QList<CustomTextEdit *> activeTextEdits;
    void initialSetup(QList<QObject *> children);
    void nextActiveRow();
    void handleKeyPress(int key);
    void handleSubmit();

private:
    QList<QString> validSolutions;
    QList<QString> validWords;
    int activeRow = 0;
    int focusedTextEdit = 0;
    void setActiveRow(int row);
};

#endif // WORDLELOGIC_H

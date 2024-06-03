#ifndef WORDLELOGIC_H
#define WORDLELOGIC_H

#include <QDebug>
#include <QFile>
#include <QList>
#include <QRandomGenerator>
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
    void handleRestart();

private:
    QList<QString> validSolutions;
    QList<QString> validWords;
    QString solution;
    int activeRow;
    int focusedTextEdit;

    QString getRandomSolution();
    void setActiveRow(int row);
    void startGame();
};

#endif // WORDLELOGIC_H

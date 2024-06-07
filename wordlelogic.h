#ifndef WORDLELOGIC_H
#define WORDLELOGIC_H

#include <QDebug>
#include <QFile>
#include <QList>
#include <QRandomGenerator>
#include <QTextStream>
#include <customlineedit.h>
#include <filelogic.h>

class WordleLogic
{
public:
    QList<CustomLineEdit *> allLineEdits;
    QList<CustomLineEdit *> activeLineEdits;

    void initialSetup(QList<QObject *> children);
    void nextActiveRow();
    void handleKeyPress(int key);
    void handleSubmit();
    void handleRestart();

private:
    QList<QString> validSolutions;
    QList<QString> validWords;
    QList<SaveFileRow> saveFile;
    FileLogic fLogic;
    QString solution;
    int activeRow;
    int focusedLineEdit;

    QString getRandomSolution();
    void setActiveRow(int row);
    void startGame();
};

#endif // WORDLELOGIC_H

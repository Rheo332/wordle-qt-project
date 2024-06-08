#ifndef WORDLELOGIC_H
#define WORDLELOGIC_H

#include <QDebug>
#include <QFile>
#include <QLabel>
#include <QList>
#include <QRandomGenerator>
#include <QTextStream>

#include "customlineedit.h"
#include "filelogic.h"

class WordleLogic
{
public:
    QList<CustomLineEdit *> allLineEdits;
    QList<CustomLineEdit *> activeLineEdits;
    QLabel *statsLabel;
    QLabel *infoLabel;
    QLabel *endLabel;
    QLabel *warningLabel;

    void initialSetup(QList<QObject *> children);
    void nextActiveRow();
    void handleKeyPress(int key);
    void handleSubmit();
    void handleRestart();
    void handleDeleteSaveFile();

private:
    QList<QString> validSolutions;
    QList<QString> validWords;
    QList<SaveFileRow> saveFile;
    QLabel *label;
    FileLogic fLogic;
    QString solution;
    int activeRow;
    int focusedLineEdit;
    bool gameInProgress;
    bool confirmDelete;

    QString getRandomSolution();
    void setActiveRow(int row);
    void startGame();
    void updateStats();
};

#endif // WORDLELOGIC_H

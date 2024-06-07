#include "wordlelogic.h"

#define validSolutionsFileString ":/valid-solutions.txt"
#define validWordsFileString ":/valid-words.txt"
#define saveDataFileString "./savedata.csv"

void WordleLogic::initialSetup(QList<QObject *> children)
{
    try {
        validSolutions = fLogic.readTextFile(validSolutionsFileString);
        validWords = fLogic.readTextFile(validWordsFileString);
        saveFile = fLogic.readCsvFile(saveDataFileString);
    } catch (const char *msg) {
        qDebug() << msg;
    } catch (QString msg) {
        qDebug() << msg;
    }

    for (auto *child : children) {
        if (CustomLineEdit *lineEdit = qobject_cast<CustomLineEdit *>(child)) {
            allLineEdits << lineEdit;
            lineEdit->setEnabled(false);
        }
    }
    startGame();
}

void WordleLogic::setActiveRow(int row)
{
    if (row > 5) {
        throw "Trying to set active row > 5";
    }

    if (row < 0) {
        for (auto edit : allLineEdits) {
            edit->setEnabled(false);
        }
        return;
    }

    activeRow = row;
    if (!activeLineEdits.isEmpty()) {
        for (auto edit : activeLineEdits) {
            edit->setEnabled(false);
        }
        activeLineEdits.clear();
    }

    int firstLineEdit = activeRow * 5;
    for (int i = firstLineEdit; i < firstLineEdit + 5; ++i) {
        activeLineEdits << allLineEdits[i];
        allLineEdits[i]->setEnabled(true);
    }

    activeLineEdits[0]->setFocus();
    focusedLineEdit = 0;
}

void WordleLogic::startGame()
{
    activeRow = 0;
    focusedLineEdit = 0;
    setActiveRow(activeRow);
    std::sort(allLineEdits.begin(), allLineEdits.end(), [](CustomLineEdit *l, CustomLineEdit *r) {
        return l->objectName() < r->objectName();
    });
    activeLineEdits[0]->setFocus();
    solution = getRandomSolution();
}

QString WordleLogic::getRandomSolution()
{
    int randomIndex = QRandomGenerator::global()->bounded(0, validSolutions.size() - 1);
    auto checkWords = [this, &randomIndex]() -> bool {
        for (const auto &row : saveFile) {
            if (row.word == validSolutions[randomIndex]) {
                return true;
            }
        }
        return false;
    };

    while (checkWords()) {
        randomIndex = QRandomGenerator::global()->bounded(0, validSolutions.size() - 1);
    }

    qDebug() << "Solution:" << validSolutions[randomIndex];
    return validSolutions[randomIndex];
}

void WordleLogic::nextActiveRow()
{
    try {
        setActiveRow(activeRow + 1);
    } catch (const char *msg) {
        qDebug() << msg;
    }
}

void WordleLogic::handleKeyPress(int key)
{
    switch (key) {
    case Qt::Key_Backspace:
        if (!activeLineEdits[focusedLineEdit]->text().isEmpty()) {
            activeLineEdits[focusedLineEdit]->setText("");
        } else {
            if (focusedLineEdit > 0) {
                focusedLineEdit--;
                activeLineEdits[focusedLineEdit]->setFocus();
                activeLineEdits[focusedLineEdit]->setText("");
            }
        }
        break;
    case Qt::Key_Return:
        handleSubmit();
        break;
    case Qt::Key_Left:
    case Qt::Key_Up:
        if (focusedLineEdit > 0) {
            focusedLineEdit--;
            activeLineEdits[focusedLineEdit]->setFocus();
        }
        break;
    case Qt::Key_Right:
    case Qt::Key_Down:
        if (focusedLineEdit < 4) {
            focusedLineEdit++;
            activeLineEdits[focusedLineEdit]->setFocus();
        }
        break;
    default:
        QChar c = (char) key;
        if (c.isLetter()) {
            c = c.toUpper();
            activeLineEdits[focusedLineEdit]->setText(c);
            activeLineEdits[focusedLineEdit]->startKeyPressAnimation();

            if (focusedLineEdit < 4) {
                focusedLineEdit++;
            }
            activeLineEdits[focusedLineEdit]->setFocus();
        }
        break;
    }
}

void WordleLogic::handleSubmit()
{
    QString word;
    for (auto lineEdit : activeLineEdits) {
        if (lineEdit->text().isEmpty() || !lineEdit->text().at(0).isLetter()) {
            for (auto lineEdit : activeLineEdits) {
                lineEdit->startWrongWordAnimation();
            }
            return;
        } else {
            word = word + lineEdit->text().at(0);
        }
    }

    word = word.toLower();
    QString tempSolution = solution;
    if (validWords.contains(word)) {
        if (word == tempSolution) {
            for (auto te : activeLineEdits) {
                te->setStyleSheet("background-color: #007700;");
                te->setEnabled(false);
            }
            SaveFileRow lastRow = saveFile.last();
            SaveFileRow newRow{solution, lastRow.games + 1, lastRow.wins + 1, lastRow.streak + 1};
            fLogic.writeCsvFile(saveDataFileString, newRow);

            // TODO: End the game ?
        } else {
            for (int i = tempSolution.size() - 1; i >= 0; --i) {
                if (word.at(i) == tempSolution.at(i)) {
                    activeLineEdits[i]->setStyleSheet("background-color: #007700;");
                    word[i] = '0';
                    tempSolution[i] = '0';
                }
            }

            for (int i = 0; i < tempSolution.size(); ++i) {
                if (tempSolution.at(i) != '0' && tempSolution.contains(word.at(i))) {
                    activeLineEdits[i]->setStyleSheet("background-color: #B0B000;");
                }
            }
            if (activeRow < 5) {
                nextActiveRow();
            } else {
                SaveFileRow lastRow = saveFile.last();
                SaveFileRow newRow{solution, lastRow.games + 1, lastRow.wins, 0};
                fLogic.writeCsvFile(saveDataFileString, newRow);
                setActiveRow(-1);

                // TODO: End the game?
            }
        }
    } else {
        for (auto lineEdit : activeLineEdits) {
            lineEdit->startWrongWordAnimation();
        }
    }
}

void WordleLogic::handleRestart()
{
    for (auto te : allLineEdits) {
        te->startResetAnimation();
    }
    saveFile = fLogic.readCsvFile(saveDataFileString);
    startGame();
}

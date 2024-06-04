#include "wordlelogic.h"

void WordleLogic::initialSetup(QList<QObject *> children)
{
    FileLogic fLogic;
    validSolutions = fLogic.readTextFile("://valid-solutions.txt");
    validWords = fLogic.readTextFile("://valid-words.txt");

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
        throw std::range_error("Trying to set active row > 5");
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
    // TODO: use saved gamestats to prevent same word twice in a row
    int randomIndex = QRandomGenerator::global()->bounded(0, validSolutions.size() - 1);
    qDebug() << "Solution:" << validSolutions[randomIndex];
    return validSolutions[randomIndex];
}

void WordleLogic::nextActiveRow()
{
    setActiveRow(activeRow + 1);
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
                // TODO: implement this animation
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

            // TODO: End the game ?
            // TODO: save game stats
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
            }
        }
    } else {
        for (auto lineEdit : activeLineEdits) {
            // TODO: implement this animation
            lineEdit->startWrongWordAnimation();
        }
        qDebug() << "not a word";
    }
}

void WordleLogic::handleRestart()
{
    for (auto te : allLineEdits) {
        te->startResetAnimation();
    }
    startGame();
}

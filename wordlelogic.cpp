#include "wordlelogic.h"

void WordleLogic::initialSetup(QList<QObject *> children)
{
    FileLogic fLogic;
    validSolutions = fLogic.readTextFile("://valid-solutions.txt");
    validWords = fLogic.readTextFile("://valid-words.txt");

    for (auto *child : children) {
        if (CustomTextEdit *textEdit = qobject_cast<CustomTextEdit *>(child)) {
            allTextEdits << textEdit;
            textEdit->setEnabled(false);
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
    if (!activeTextEdits.isEmpty()) {
        for (auto edit : activeTextEdits) {
            edit->setEnabled(false);
        }
        activeTextEdits.clear();
    }

    int firstTextEdit = activeRow * 5;
    for (int i = firstTextEdit; i < firstTextEdit + 5; ++i) {
        activeTextEdits << allTextEdits[i];
        allTextEdits[i]->setEnabled(true);
    }

    activeTextEdits[0]->setFocus();
    focusedTextEdit = 0;
}

void WordleLogic::startGame()
{
    activeRow = 0;
    focusedTextEdit = 0;
    setActiveRow(activeRow);
    std::sort(allTextEdits.begin(), allTextEdits.end(), [](CustomTextEdit *l, CustomTextEdit *r) {
        return l->objectName() < r->objectName();
    });
    activeTextEdits[0]->setFocus();
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
        if (!activeTextEdits[focusedTextEdit]->toPlainText().isEmpty()) {
            activeTextEdits[focusedTextEdit]->setPlainText("");
        } else {
            if (focusedTextEdit > 0) {
                focusedTextEdit--;
                activeTextEdits[focusedTextEdit]->setFocus();
                activeTextEdits[focusedTextEdit]->setPlainText("");
            }
        }
        break;
    case Qt::Key_Return:
        handleSubmit();
        break;
    case Qt::Key_Left:
    case Qt::Key_Up:
        if (focusedTextEdit > 0) {
            focusedTextEdit--;
            activeTextEdits[focusedTextEdit]->setFocus();
        }
        break;
    case Qt::Key_Right:
    case Qt::Key_Down:
        if (focusedTextEdit < 4) {
            focusedTextEdit++;
            activeTextEdits[focusedTextEdit]->setFocus();
        }
        break;
    default:
        QChar c = (char) key;
        if (c.isLetter()) {
            c = c.toUpper();
            activeTextEdits[focusedTextEdit]->setPlainText(c);
            if (focusedTextEdit < 4) {
                focusedTextEdit++;
            }
            activeTextEdits[focusedTextEdit]->setFocus();
        }
        break;
    }
}

void WordleLogic::handleSubmit()
{
    QString word;
    for (auto textEdit : activeTextEdits) {
        if (textEdit->toPlainText().isEmpty() || !textEdit->toPlainText().at(0).isLetter()) {
            // not all 5 textEdits have a letter

            // TODO: visualize wrong word

            return;
        } else {
            word = word + textEdit->toPlainText().at(0);
        }
    }

    word = word.toLower();
    QString tempSolution = solution;
    if (validWords.contains(word)) {
        if (word == tempSolution) {
            for (auto te : activeTextEdits) {
                te->setStyleSheet("background-color: #007700;");
                te->setEnabled(false);
            }
            // TODO: End the game ?
            // TODO: save game stats
        } else {
            for (int i = tempSolution.size() - 1; i >= 0; --i) {
                if (word.at(i) == tempSolution.at(i)) {
                    activeTextEdits[i]->setStyleSheet("background-color: #007700;");
                    word[i] = '0';
                    tempSolution[i] = '0';
                }
            }

            for (int i = 0; i < tempSolution.size(); ++i) {
                if (tempSolution.at(i) != '0' && tempSolution.contains(word.at(i))) {
                    activeTextEdits[i]->setStyleSheet("background-color: #B0B000;");
                }
            }
            nextActiveRow();
        }
    } else {
        // TODO: visualize wrong word
        qDebug() << "not a word";
    }
}

void WordleLogic::handleRestart()
{
    for (auto te : allTextEdits) {
        te->setPlainText("");
        te->setStyleSheet("");
        te->setEnabled(false);
    }
    startGame();
}

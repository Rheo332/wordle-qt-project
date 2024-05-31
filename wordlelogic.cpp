#include "wordlelogic.h"

void WordleLogic::initialSetup(QList<QObject *> children)
{
    for (auto *child : children) {
        if (CustomTextEdit *textEdit = qobject_cast<CustomTextEdit *>(child)) {
            allTextEdits << textEdit;
            textEdit->setEnabled(false);
        }
    }

    setActiveRow(activeRow);
    std::sort(allTextEdits.begin(), allTextEdits.end(), [](CustomTextEdit *l, CustomTextEdit *r) {
        return l->objectName() < r->objectName();
    });

    activeTextEdits[0]->setFocus();

    FileLogic fLogic;
    validSolutions = fLogic.readTextFile("://valid-solutions.txt");
    validWords = fLogic.readTextFile("://valid-words.txt");

    solution = getRandomSolution();
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

QString WordleLogic::getRandomSolution()
{
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
        activeTextEdits[focusedTextEdit]->setPlainText("");
        if (focusedTextEdit > 0) {
            focusedTextEdit--;
            activeTextEdits[focusedTextEdit]->setFocus();
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
            return;
        } else {
            word = word + textEdit->toPlainText().at(0);
        }
    }

    if (validWords.contains(word.toLower())) {
        //TODO: check word

        nextActiveRow();
    } else {
        qDebug() << "not a word";
    }
}

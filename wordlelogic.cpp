#include "wordlelogic.h"

#include <QMainWindow>

#define validSolutionsFileString ":/valid-solutions.txt"
#define validWordsFileString ":/valid-words.txt"
#define saveDataFileString "./savedata.csv"
#define getVariableName(Variable) (void(Variable), #Variable)

WordleLogic::~WordleLogic()
{
    delete statsLabel;
    delete infoLabel;
    delete endLabel;
    delete warningLabel;
}

void WordleLogic::initialSetup(QList<QObject *> children)
{
    try {
        validSolutions = fLogic.readTextFile(validSolutionsFileString);
        validWords = fLogic.readTextFile(validWordsFileString);
        saveFile = fLogic.readCsvFile(saveDataFileString);
    } catch (const std::runtime_error &e) {
        qDebug() << e.what();
    }

    for (auto *child : children) {
        if (CustomLineEdit *lineEdit = qobject_cast<CustomLineEdit *>(child)) {
            allLineEdits << lineEdit;
            lineEdit->setEnabled(false);
        } else if (QLabel *labelEdit = qobject_cast<QLabel *>(child)) {
            QString name = labelEdit->objectName();
            if (name == QString(getVariableName(endLabel))) {
                endLabel = labelEdit;
            } else if (name == QString(getVariableName(infoLabel))) {
                infoLabel = labelEdit;
            } else if (name == QString(getVariableName(statsLabel))) {
                statsLabel = labelEdit;
            } else if (name == QString(getVariableName(warningLabel))) {
                warningLabel = labelEdit;
            }
        }
    }
    warningLabel->setWordWrap(true);
    warningLabel->setText("Warning: This will delete your save file! Continue?");
    warningLabel->hide();

    infoLabel->setWordWrap(true);
    infoLabel->setText(
        "Use Keyboard to type words and arrow keys to navigate\n\nCtrl + R to restart the game");
    startGame();
}

void WordleLogic::setActiveRow(int row)
{
    if (row > 5) {
        throw std::logic_error(QString("Trying to set active row to %1").arg(row).toStdString());
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
    gameInProgress = true;
    updateStats();
    activeRow = 0;
    focusedLineEdit = 0;
    setActiveRow(activeRow);
    std::sort(allLineEdits.begin(), allLineEdits.end(), [](CustomLineEdit *l, CustomLineEdit *r) {
        return l->objectName() < r->objectName();
    });
    activeLineEdits[0]->setFocus();
    solution = getRandomSolution();
}

void WordleLogic::updateStats()
{
    try {
        saveFile = fLogic.readCsvFile(saveDataFileString);
    } catch (const std::runtime_error &e) {
        qDebug() << e.what();
    }

    if (saveFile.size() >= 1) {
        SaveFileRow lastSaveFileRow = saveFile.last();
        QString gameStats = QString("Games:\t%1\n"
                                    "Wins:\t%2\n"
                                    "Streak:\t%3")
                                .arg(lastSaveFileRow.games)
                                .arg(lastSaveFileRow.wins)
                                .arg(lastSaveFileRow.streak);
        statsLabel->setText(gameStats);
    }
}

QString WordleLogic::getRandomSolution()
{
    int randomIndex = QRandomGenerator::global()->bounded(0, validSolutions.size() - 1);
    auto checkDuplicateWords = [this, &randomIndex]() -> bool {
        for (const auto &row : saveFile) {
            if (row.word == validSolutions[randomIndex]) {
                return true;
            }
        }
        return false;
    };

    while (checkDuplicateWords()) {
        randomIndex = QRandomGenerator::global()->bounded(0, validSolutions.size() - 1);
    }

    qDebug() << "Solution:" << validSolutions[randomIndex];
    return validSolutions[randomIndex];
}

void WordleLogic::nextActiveRow()
{
    try {
        setActiveRow(activeRow + 1);
    } catch (const std::logic_error &e) {
        qDebug() << e.what();
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
            try {
                fLogic.writeCsvFile(saveDataFileString, newRow);
            } catch (const std::runtime_error &e) {
                qDebug() << e.what();
            }

            updateStats();
            gameInProgress = false;
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
                try {
                    fLogic.writeCsvFile(saveDataFileString, newRow);
                } catch (const std::runtime_error &e) {
                    qDebug() << e.what();
                }
                setActiveRow(-1);
                updateStats();
                endLabel->setText(QString("The word was %1").arg(solution));
                gameInProgress = false;
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
    if (gameInProgress) {
        SaveFileRow lastRow = saveFile.last();
        SaveFileRow newRow{solution, lastRow.games + 1, lastRow.wins, 0};
        try {
            fLogic.writeCsvFile(saveDataFileString, newRow);
        } catch (const std::runtime_error &e) {
            qDebug() << e.what();
        }
    }
    for (auto te : allLineEdits) {
        te->startResetAnimation();
    }
    endLabel->setText("");
    warningLabel->hide();
    confirmDelete = false;
    startGame();
}

void WordleLogic::handleDeleteSaveFile()
{
    if (!confirmDelete) {
        warningLabel->show();
        confirmDelete = true;
    } else {
        handleRestart();
        fLogic.deleteCsvFile(saveDataFileString);
        updateStats();
    }
}

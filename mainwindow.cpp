#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <list>

#define rowString(x) QString::fromStdString("_" + std::to_string(x) + "_")

std::list<QTextEdit *> textRow;
int activeRow = 1;

void input(QTextEdit *textEdit);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    foreach (QObject *child, ui->centralwidget->children()) {
        if (QTextEdit *textEdit = qobject_cast<QTextEdit *>(child)) {
            connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::handleTextChanged);
            if (textEdit->objectName().contains(rowString(activeRow))) {
                textRow.push_back(textEdit);
            } else {
                textEdit->setEnabled(false);
            }

            // qDebug() << "Connected " << textEdit->objectName() << " to \"handleTextChanged()\"";
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleTextChanged()
{
    if (QTextEdit *textEdit = qobject_cast<QTextEdit *>(sender())) {
        input(textEdit);
        auto te = std::find(textRow.begin(), textRow.end(), textEdit);
        if (te != textRow.end()) {
            auto dist = std::distance(textRow.begin(), te);
            if (dist < 4) {
                // qDebug() << dist;
                //ui->textEdit_1_1->setFocus();
            }
        }
    }
}

void MainWindow::on_pushButton_clicked() {}

void input(QTextEdit *textEdit)
{
    QString content = textEdit->toPlainText();

    if (content.size() == 0) {
        return;
    }

    if (content.size() == 1 && !content.at(0).isLetter()) {
        textEdit->setPlainText("");
        return;
    }

    if (content.size() > 1 && !content.at(1).isLetter()) {
        textEdit->setPlainText(content.at(0));

        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        textEdit->setTextCursor(cursor);
        return;
    }

    if (content.size() > 1) {
        textEdit->setPlainText(content.at(content.size() - 1).toUpper());
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        textEdit->setTextCursor(cursor);
    } else if (content.at(0).isLower()) {
        textEdit->setPlainText(content.toUpper());
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        textEdit->setTextCursor(cursor);
    }
}

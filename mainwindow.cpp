#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <list>

#define rowString(x) QString::fromStdString("_" + std::to_string(x) + "_")
#define rowColTextEdit(r, c) textEdit_r_c

std::list<QTextEdit *> textRow;
int activeRow = 1;

QString input(QTextEdit *textEdit);

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
        QString content = input(textEdit);
        auto te = std::find(textRow.begin(), textRow.end(), textEdit);
        if (te != textRow.end()) {
            auto dist = std::distance(textRow.begin(), te);
            if (dist < 4 && content != "Del") {
                // qDebug() << dist;

                auto i = textRow.begin();
                std::advance(i, dist + 1);
                auto nextTe = *i;
                nextTe->setFocus();
            } else if (dist > 0 && content == "Del") {
                auto i = textRow.begin();
                std::advance(i, dist - 1);
                auto nextTe = *i;
                nextTe->setFocus();
            }
        }
    }
}

void MainWindow::on_pushButton_clicked() {}

QString input(QTextEdit *textEdit)
{
    QString content = textEdit->toPlainText();

    if (content.size() == 0) {
        return "Del";
    }

    if (content.size() == 1 && !content.at(0).isLetter()) {
        textEdit->setPlainText("");
        return "";
    }

    if (content.size() > 1 && !content.at(1).isLetter()) {
        content = content.at(0);
        textEdit->setPlainText(content);

        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        textEdit->setTextCursor(cursor);
        return content;
    }

    if (content.size() > 1) {
        content = content.at(content.size() - 1).toUpper();
        textEdit->setPlainText(content);
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        textEdit->setTextCursor(cursor);
        return content;
    } else if (content.at(0).isLower()) {
        content = content.toUpper();
        textEdit->setPlainText(content);
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        textEdit->setTextCursor(cursor);
        return content;
    }

    return "Del";
}

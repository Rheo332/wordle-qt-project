#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

void input(QTextEdit *textEdit);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    foreach (QObject *child, ui->centralwidget->children()) {
        if (QTextEdit *textEdit = qobject_cast<QTextEdit *>(child)) {
            connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::handleTextChanged);
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
        cursor.setPosition(1);
        textEdit->setTextCursor(cursor);
        return;
    }

    if (content.size() > 1) {
        textEdit->setPlainText(QString(content.at(content.size() - 1)).toUpper());
        QTextCursor cursor = textEdit->textCursor();
        cursor.setPosition(1);
        textEdit->setTextCursor(cursor);
    } else if (content.at(0).isLower()) {
        textEdit->setPlainText(content.toUpper());
        QTextCursor cursor = textEdit->textCursor();
        cursor.setPosition(1);
        textEdit->setTextCursor(cursor);
    }
}

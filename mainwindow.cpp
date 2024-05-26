#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

CustomTextEdit *activeTextEdits[5];
int activeRow = 1;
int focusedTextEdit = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    foreach (QObject *child, ui->centralwidget->children()) {
        if (CustomTextEdit *textEdit = qobject_cast<CustomTextEdit *>(child)) {
            connect(textEdit, &CustomTextEdit::keyPress, this, &MainWindow::onKeyPress);
            if (textEdit->objectName().contains(
                    QString::fromStdString("_" + std::to_string(activeRow) + "_"))) {
                activeTextEdits[textEdit->objectName().last(1).toInt() - 1] = textEdit;
            } else {
                textEdit->setEnabled(false);
            }
        }
    }

    ui->textEdit_1_1->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() {}

void MainWindow::onKeyPress(QKeyEvent *event)
{
    QChar c = (char) event->key();
    switch (event->key()) {
    case Qt::Key_Backspace:
        qDebug() << "Backspace";
        activeTextEdits[focusedTextEdit]->setPlainText("");
        if (focusedTextEdit > 0) {
            focusedTextEdit--;
            activeTextEdits[focusedTextEdit]->setFocus();
        }
        break;
    case Qt::Key_Return:
        qDebug() << "Enter";

        // TODO

        break;
    case Qt::Key_Left:
        qDebug() << "Left";
        if (focusedTextEdit > 0) {
            focusedTextEdit--;
            activeTextEdits[focusedTextEdit]->setFocus();
        }
        break;
    case Qt::Key_Up:
        qDebug() << "Up";
        if (focusedTextEdit > 0) {
            focusedTextEdit--;
            activeTextEdits[focusedTextEdit]->setFocus();
        }
        break;
    case Qt::Key_Right:
        qDebug() << "Right";
        if (focusedTextEdit < 4) {
            focusedTextEdit++;
            activeTextEdits[focusedTextEdit]->setFocus();
        }
        break;
    case Qt::Key_Down:
        qDebug() << "Down";
        if (focusedTextEdit < 4) {
            focusedTextEdit++;
            activeTextEdits[focusedTextEdit]->setFocus();
        }
        break;
    default:
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

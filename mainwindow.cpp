#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <wordlelogic.h>

WordleLogic wl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wl.initialSetup(ui->centralwidget->children());

    for (auto textEdit : wl.allTextEdits) {
        connect(textEdit, &CustomTextEdit::keyPressSignal, this, &MainWindow::onKeyPress);
    }
    connect(ui->submitButton, &QPushButton::clicked, this, &MainWindow::onSubmitClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onKeyPress(QKeyEvent *event)
{
    wl.handleKeyPress(event->key());
}

void MainWindow::onSubmitClicked()
{
    wl.handleSubmit();
}

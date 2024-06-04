#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <customlineedit.h>
#include <wordlelogic.h>

WordleLogic wl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wl.initialSetup(ui->centralwidget->children());

    for (auto lineEdit : wl.allLineEdits) {
        connect(lineEdit, &CustomLineEdit::keyPressSignal, this, &MainWindow::onKeyPress);
        lineEdit->setupAnimation();
    }
    connect(ui->submitButton, &QPushButton::clicked, this, &MainWindow::onSubmitClicked);
    connect(ui->restartButton, &QPushButton::clicked, this, &MainWindow::onRestartClicked);
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

void MainWindow::onRestartClicked()
{
    wl.handleRestart();
}

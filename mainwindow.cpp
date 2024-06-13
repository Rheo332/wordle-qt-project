#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "customlineedit.h"
#include "wordlelogic.h"

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
    restartShortcut = new QShortcut(QKeySequence("Ctrl+R"), this);
    connect(restartShortcut, &QShortcut::activated, this, &MainWindow::onRestartClicked);
    connect(ui->restartButton, &QPushButton::clicked, this, &MainWindow::onRestartClicked);
    connect(ui->deleteStatsButton, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete restartShortcut;
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

void MainWindow::onDeleteClicked()
{
    wl.handleDeleteSaveFile();
}

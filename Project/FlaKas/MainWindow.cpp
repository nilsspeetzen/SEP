#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @file mainwindow.cpp
 * @brief Main Window
 * @param parent
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::setProgress(int i)
{
    ui->progressBar->setValue(i);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OKButton_clicked()
{
    ui->OKButton->hide();
}

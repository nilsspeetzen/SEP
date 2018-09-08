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

MainWindow::~MainWindow()
{
    delete ui;
}

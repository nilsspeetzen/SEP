#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @file mainwindow.cpp
 * @brief Hauptfenster
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

void MainWindow::addButton()
{
    QWidget * wdg = new QWidget(this);
    setCentralWidget(wdg); //ich glaube CentralWidget ist alles was im Fenster und nicht in den oberen Menüs ist
}

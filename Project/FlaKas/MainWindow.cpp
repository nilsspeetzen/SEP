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

void MainWindow::addButton()
{
    QWidget * wdg = new QWidget(this);
    setCentralWidget(wdg); //ich glaube CentralWidget ist alles was im Fenster und nicht in den oberen Menüs ist
}

//ein slot, wenn man im design window auf einen button rechtsklickt dann kann man dazu gehen was passiert wenn er z.b. geklickt wird
void MainWindow::on_OKButton_clicked()
{
    ui->OKButton->hide();
}

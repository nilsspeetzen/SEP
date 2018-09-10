#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {

class MainWindow;
}

/**
 *@brief Hauptfenster
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setProgress(int i);

private slots:
    void on_OKButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {

class MainWindow;
}

/**
 *@brief Hauptfenster
 */

//TODO herausfinden, wie verschiedene Elemente hinzugefügt werden können / wie wir ein veränderbares Fließdiagramm machen können

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addButton(); //test, macht momentan das Fenster leer

private slots:
    void on_OKButton_clicked();

private:
    Ui::MainWindow *ui; //Inhalt des gesamten Fensters
};

#endif // MAINWINDOW_H

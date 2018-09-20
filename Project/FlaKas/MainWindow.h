#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/**
 * @file mainwindow.h
 * @brief Hauptfenster
 */

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
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addButton(); //test, macht momentan das Fenster leer

    QString userName() {
        return m_userName;
    }
    void setUserName(QString newUserName) {
        m_userName = newUserName;
        emit userNameChanged();
    }

signals:
    void userNameChanged();

private slots:

private:
    QString m_userName;
    Ui::MainWindow *ui; //Inhalt des gesamten Fensters
};

#endif // MAINWINDOW_H

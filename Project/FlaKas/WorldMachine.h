#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include "cascade.h"

/**
 * @file mainwindow.h
 * @brief Hauptfenster
 */

namespace Ui {

class WorldMachine;
}

/**
 *@brief Hauptfenster
 */

//TODO herausfinden, wie verschiedene Elemente hinzugefügt werden können / wie wir ein veränderbares Fließdiagramm machen können

class WorldMachine : public QObject
{
    Q_OBJECT

private:
    QString m_userName;
    QObject* _root;

public:
    WorldMachine(QObject* root) : _root(root) {}

signals:

public slots:
    void cppSlot(const QString& msg) {
        qDebug() << msg;
        _root->findChild<QObject*>("row")->findChild<QObject*>("tf")->setProperty("number", 2);
    }

    void startOneFlashSlot() {
        static int NUMSUBSTANCES = 2;
        static Matrix<double,2,7> A;
        A <<    73.649, -7258.2, 0, 0, -7.3037, 4.1653e-6, 2,
                79.276, -10105, 0, 0, -7.521, 7.3408e-19, 6;
        cascade<> C(NUMSUBSTANCES, A);
        C.addFlash();
        QR<> lsol;
        MODULENEWTON<> nlsol(lsol);
        nlsol.eps() = 1;
        nlsol.solve(C.getFlash(0));
        std::cout << "x: " << std::endl << C.getFlash(0).f() << std::endl;
    }
};

#endif // MAINWINDOW_H

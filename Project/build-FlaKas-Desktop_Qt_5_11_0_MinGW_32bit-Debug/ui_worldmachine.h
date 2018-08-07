/********************************************************************************
** Form generated from reading UI file 'worldmachine.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORLDMACHINE_H
#define UI_WORLDMACHINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WorldMachine
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WorldMachine)
    {
        if (WorldMachine->objectName().isEmpty())
            WorldMachine->setObjectName(QStringLiteral("WorldMachine"));
        WorldMachine->resize(400, 300);
        menuBar = new QMenuBar(WorldMachine);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        WorldMachine->setMenuBar(menuBar);
        mainToolBar = new QToolBar(WorldMachine);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        WorldMachine->addToolBar(mainToolBar);
        centralWidget = new QWidget(WorldMachine);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        WorldMachine->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(WorldMachine);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        WorldMachine->setStatusBar(statusBar);

        retranslateUi(WorldMachine);

        QMetaObject::connectSlotsByName(WorldMachine);
    } // setupUi

    void retranslateUi(QMainWindow *WorldMachine)
    {
        WorldMachine->setWindowTitle(QApplication::translate("WorldMachine", "WorldMachine", nullptr));
    } // retranslateUi

};

namespMainWindow class WorldMachine: public Ui_WorldMachine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORLDMACHINE_H

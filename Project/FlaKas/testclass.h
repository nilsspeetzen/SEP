#ifndef TESTCLASS_H
#define TESTCLASS_H
#include "mainwindow.h"

class TestClass
{
public:
    TestClass();
    int helloTestClass(){
        return 50;
    }
    void makeNewButton(MainWindow* mW){
        mW->addButton();
    }
};

#endif // TESTCLASS_H

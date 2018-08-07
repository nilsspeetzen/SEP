#include "worldmachine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WorldMachine w;
    w.show();

    return a.exec();
}

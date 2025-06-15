#include <QApplication>
#include "interface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Interface * interface = new Interface;

    mousePressEventFilter * filter = new mousePressEventFilter(interface->scene, interface);
    a.installEventFilter(filter);

    return a.exec();
}

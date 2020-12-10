#include "tempwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TempWindow w;
    w.show();
    return a.exec();
}

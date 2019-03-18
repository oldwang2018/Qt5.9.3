#include "qwmainwind.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWmainWind w;
    w.show();

    return a.exec();
}

#include "qarduinodialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QArduinoDialog w;
    w.show();

    return a.exec();
}

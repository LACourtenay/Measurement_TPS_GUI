#include "Measurement_Software.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Measurement_Software w;
    w.show();
    return a.exec();
}

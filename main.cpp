#include "mines.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMines w;
    w.show();
    return a.exec();
}

#include "mazeserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mazeserver w;
    w.show();
    return a.exec();
}

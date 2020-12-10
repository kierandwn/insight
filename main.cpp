#include "include/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InsightMainWindow w;
    w.show();

    return a.exec();
}

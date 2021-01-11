#include "lib/layout/include/mainwindow.h"

#include <string>

#include <QApplication>

using namespace std;


int main(int argc, char *argv[])
{
    string file_path = __FILE__;
    string dir_path = file_path.substr(0, file_path.rfind("\\"));

    QApplication a(argc, argv);
    InsightMainWindow w(dir_path);
    w.show();

    return a.exec();
}

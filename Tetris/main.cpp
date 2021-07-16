// Danie Walker 26846179
// Johnny Scholtz 31614795
// REII 313 Practical

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

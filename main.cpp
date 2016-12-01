#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    QGuiApplication::setQuitOnLastWindowClosed(true);
    QApplication::setQuitOnLastWindowClosed(true);
    MainWindow w;
    w.setAttribute(Qt::WA_QuitOnClose);
    w.show();

    return a.exec();
}

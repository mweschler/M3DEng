#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    M3DEditGUI::MainWindow w;
    w.show();
    
    return a.exec();
}

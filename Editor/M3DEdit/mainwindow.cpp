#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace M3DEditGUI{
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
}

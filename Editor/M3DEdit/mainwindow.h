#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "axisrenderer.h"
#include "geometrymanager.h"

namespace Ui {
class MainWindow;
}
namespace M3DEditGUI{
//! the main window of the editor
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    M3DEditLevel::GeometryManager *getGeoMgr();
    M3DEditRender::AxisRenderer *getAxisRender();

private slots:
    void on_actionAddGeo_triggered();

private:
    M3DEditRender::AxisRenderer axisRenderer;
    M3DEditLevel::GeometryManager geoMgr;
    Ui::MainWindow *ui;
};
}

#endif // MAINWINDOW_H

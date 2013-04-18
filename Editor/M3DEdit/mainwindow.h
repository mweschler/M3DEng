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
    int getSelected();

private slots:
    void on_actionAddGeo_triggered();
    void selectGeo(int id);


    void on_actionCamera_toggled(bool arg1);

    void on_actionBrush_toggled(bool arg1);

signals:
    void draw();

private:
    M3DEditRender::AxisRenderer axisRenderer;
    M3DEditLevel::GeometryManager geoMgr;
    Ui::MainWindow *ui;
    int selected;
    bool cameraButton;
    bool brushButton;
};

extern MainWindow *mainWnd;
}

#endif // MAINWINDOW_H

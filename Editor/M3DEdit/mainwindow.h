#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
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

protected:
    virtual void keyReleaseEvent(QKeyEvent *event);

private slots:
    void on_actionAddGeo_triggered();
    void selectGeo(int id);


    void on_actionCamera_toggled(bool arg1);

    void on_actionBrush_toggled(bool arg1);

    void on_rSpin_valueChanged(double arg1);

    void on_gSpin_valueChanged(double arg1);

    void on_bSpin_valueChanged(double arg1);

    void on_aSpin_valueChanged(double arg1);

signals:
    void draw();

private:
    QVector4D getRBGA();
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

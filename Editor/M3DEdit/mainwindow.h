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

    //! gets the id of the currently selected piece of geometry, -1 for none
    int getSelected();

protected:
    //! handles key release events
    virtual void keyReleaseEvent(QKeyEvent *event);

private slots:
    //! slot to handle the test funciton of adding geometry
    void on_actionAddGeo_triggered();

    //! select the geometry of id given
    void selectGeo(int id);

    //! camera button was toggled
    void on_actionCamera_toggled(bool arg1);

    //! brush button was toggled
    void on_actionBrush_toggled(bool arg1);

    //! red spinnger was changed
    void on_rSpin_valueChanged(double arg1);

    //! green spinner was changed
    void on_gSpin_valueChanged(double arg1);

    //! blue spinner was changed
    void on_bSpin_valueChanged(double arg1);

    //! alpha spinner was changed
    void on_aSpin_valueChanged(double arg1);

signals:
    //! widgets should draw themselves
    void draw();

private:
    //! gets the current RGBA value of the spinners as a vector
    QVector4D getRBGA();

    //! qt designer UI
    Ui::MainWindow *ui;

    //! id of selected geometry
    int selected;

    //! state of the camera button
    bool cameraButton;

    //! state of the brush button
    bool brushButton;
};

//! global pointer for this window
extern MainWindow *mainWnd;
}

#endif // MAINWINDOW_H

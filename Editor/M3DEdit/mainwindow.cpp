#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "box.h"
#include "materialmanager.h"

namespace M3DEditGUI{
MainWindow *mainWnd = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    cameraButton = false;
    brushButton = false;

    //create and set global for geometry manager
    M3DEditLevel::g_geoMgr = new M3DEditLevel::GeometryManager();

    //create and set global for axis renderer
    M3DEditRender::g_axisRenderer = new M3DEditRender::AxisRenderer();

    //create and set global for material manager
    M3DEditGUI::g_MatMgr = new MaterialManager();

    //setup the designer created UI
    ui->setupUi(this);

    //initialize the views locks
    ui->axisXY->setAxisLock(M3DEditRender::XY);
    ui->axisXZ->setAxisLock(M3DEditRender::XZ);
    ui->axisYZ->setAxisLock(M3DEditRender::YZ);

    //default of nothing selected
    this->selected = -1;

    //Connect widgets singals and slots
    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryAdded(int,M3DEditLevel::Geometry*)),
                     ui->axisXY, SLOT(addGeometry(int,M3DEditLevel::Geometry*)));
    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryAdded(int,M3DEditLevel::Geometry*)),
                     ui->axisXZ, SLOT(addGeometry(int,M3DEditLevel::Geometry*)));
    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryAdded(int,M3DEditLevel::Geometry*)),
                     ui->axisYZ, SLOT(addGeometry(int,M3DEditLevel::Geometry*)));

    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryRemoved(int,M3DEditLevel::Geometry*)),
                     ui->axisXY, SLOT(removeGeometry(int,M3DEditLevel::Geometry*)));
    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryRemoved(int,M3DEditLevel::Geometry*)),
                     ui->axisXZ, SLOT(removeGeometry(int,M3DEditLevel::Geometry*)));
    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryRemoved(int,M3DEditLevel::Geometry*)),
                     ui->axisYZ, SLOT(removeGeometry(int,M3DEditLevel::Geometry*)));

    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryUpdated(int,M3DEditLevel::Geometry*)),
                     ui->axisXY, SLOT(updateGeometry(int,M3DEditLevel::Geometry*)));
    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryUpdated(int,M3DEditLevel::Geometry*)),
                     ui->axisXZ, SLOT(updateGeometry(int,M3DEditLevel::Geometry*)));
    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryUpdated(int,M3DEditLevel::Geometry*)),
                     ui->axisYZ, SLOT(updateGeometry(int,M3DEditLevel::Geometry*)));

    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryAdded(int,M3DEditLevel::Geometry*)),
                     ui->perspective, SLOT(addGeometry(int,M3DEditLevel::Geometry*)));
    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryRemoved(int,M3DEditLevel::Geometry*)),
                     ui->perspective, SLOT(removeGeometry(int,M3DEditLevel::Geometry*)));
    QObject::connect(M3DEditLevel::g_geoMgr, SIGNAL(geometryUpdated(int,M3DEditLevel::Geometry*)),
                     ui->perspective, SLOT(updateGeometry(int,M3DEditLevel::Geometry*)));

    QObject::connect(ui->axisXY, SIGNAL(selectedGeo(int)), this, SLOT(selectGeo(int)));
    QObject::connect(ui->axisXZ, SIGNAL(selectedGeo(int)), this, SLOT(selectGeo(int)));
    QObject::connect(ui->axisYZ, SIGNAL(selectedGeo(int)), this, SLOT(selectGeo(int)));

    QObject::connect(this, SIGNAL(draw()), ui->axisXY, SLOT(updateGL()));
    QObject::connect(this, SIGNAL(draw()), ui->axisXZ, SLOT(updateGL()));
    QObject::connect(this, SIGNAL(draw()), ui->axisYZ, SLOT(updateGL()));

    //set global pointer for this window
    mainWnd = this;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete M3DEditRender::g_axisRenderer;
    delete M3DEditLevel::g_geoMgr;
}



int MainWindow::getSelected()
{
    return this->selected;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    //check for delete key and if so remove selected geometry
    if(event->key() == Qt::Key_Delete && this->selected != -1)
    {
        qDebug()<<"[MainWnd] Removing geo:"<<selected;
        M3DEditLevel::g_geoMgr->removeGeometry(selected);
        selected = -1;
    }
}

QVector4D MainWindow::getRBGA()
{
    return QVector4D((float)ui->rSpin->value(),(float)ui->gSpin->value(), (float)ui->bSpin->value(), (float)ui->aSpin->value());
}



void MainWindow::on_actionAddGeo_triggered()
{
    //add some test geometry
    qDebug()<<"Adding Geo!";

    QVector<QVector3D> bounds;
    bounds.push_back(QVector3D(20, 20, 20));
    bounds.push_back(QVector3D(10, 10, 10));
    M3DEditLevel::Geometry *geo = new M3DEditLevel::Box(bounds);
    M3DEditLevel::g_geoMgr->addGeometry(geo);

    QVector<QVector3D> b2;
    b2.push_back(QVector3D(30,30,-10));
    b2.push_back(QVector3D(-20,-20,-20));
    M3DEditLevel::Geometry *geo2 = new M3DEditLevel::Box(b2);
    M3DEditLevel::g_geoMgr->addGeometry(geo2);

}

void MainWindow::selectGeo(int id)
{
    this->selected = id;

    //something was selected, update properties
    if(id >= 0){
        ui->geoID->setText(QString::number(id));
        Material mat = g_MatMgr->getMaterial(id);
        QVector4D diffuse = mat.getDiffuseColor();
        ui->rSpin->setValue(diffuse.x());
        ui->gSpin->setValue(diffuse.y());
        ui->bSpin->setValue(diffuse.z());
        ui->aSpin->setValue(diffuse.w());
    }else{//unselected, set default for properties
        ui->geoID->setText(QString());
        ui->rSpin->setValue(1);
        ui->gSpin->setValue(1);
        ui->bSpin->setValue(1);
        ui->aSpin->setValue(1);
    }
        qDebug()<<"[MainWnd] selected geo"<<id;

        //have views redraw
        ui->axisXY->paintGL();
        ui->axisXZ->paintGL();
        ui->axisYZ->paintGL();
}



void MainWindow::on_actionCamera_toggled(bool arg1)
{
    cameraButton = arg1;

    if(cameraButton && brushButton)
        ui->actionBrush->toggle();
}

void MainWindow::on_actionBrush_toggled(bool arg1)
{
    brushButton = arg1;

    if(cameraButton && brushButton)
        ui->actionCamera->toggle();
}


void MainWindow::on_rSpin_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    Material mat;

    mat.setDiffuseColor(this->getRBGA());

    if(selected >= 0)
        g_MatMgr->addMaterial(selected, mat );

    ui->perspective->paintGL();
}

void MainWindow::on_gSpin_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    Material mat;

    mat.setDiffuseColor(this->getRBGA());

    if(selected >= 0)
        g_MatMgr->addMaterial(selected, mat );

    ui->perspective->paintGL();
}

void MainWindow::on_bSpin_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    Material mat;

    mat.setDiffuseColor(this->getRBGA());

    if(selected >= 0)
        g_MatMgr->addMaterial(selected, mat );

    ui->perspective->paintGL();
}

void MainWindow::on_aSpin_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    Material mat;

    mat.setDiffuseColor(this->getRBGA());

    if(selected >= 0)
        g_MatMgr->addMaterial(selected, mat );

    ui->perspective->paintGL();
}
}

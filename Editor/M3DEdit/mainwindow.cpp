#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "box.h"

namespace M3DEditGUI{
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    axisRenderer(&geoMgr),
    ui(new Ui::MainWindow)
{
    M3DEditLevel::g_geoMgr = new M3DEditLevel::GeometryManager();
    M3DEditRender::g_axisRenderer = new M3DEditRender::AxisRenderer(M3DEditLevel::g_geoMgr);
    ui->setupUi(this);

    ui->axisXY->setAxisLock(M3DEditRender::XY);
    ui->axisXZ->setAxisLock(M3DEditRender::XZ);
    ui->axisYZ->setAxisLock(M3DEditRender::YZ);

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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete M3DEditRender::g_axisRenderer;
    delete M3DEditLevel::g_geoMgr;
}

M3DEditLevel::GeometryManager *MainWindow::getGeoMgr()
{
    return &geoMgr;
}

M3DEditRender::AxisRenderer *MainWindow::getAxisRender()
{
    return &axisRenderer;
}
}

void M3DEditGUI::MainWindow::on_actionAddGeo_triggered()
{
    qDebug()<<"Adding Geo!";
    M3DEditLevel::Geometry *geo = new M3DEditLevel::Box();
    int id = M3DEditLevel::g_geoMgr->addGeometry(geo);

    QVector<QVector3D> bounds;
    bounds.push_back(QVector3D(2,2,2));
    bounds.push_back(QVector3D(3,3,3));
    geo->setBounds(bounds);

    qDebug()<<"Updating Geo";
    M3DEditLevel::g_geoMgr->updateGeometry(id, geo);

    qDebug()<<"Removing Geo";
    M3DEditLevel::g_geoMgr->removeGeometry(id);
    delete geo;
}

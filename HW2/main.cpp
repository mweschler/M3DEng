#include <QApplication>
#include <QTimer>
#include <QFileDialog>
#include <QString>
#include <QInputDialog>
#include <QMessageBox>
#include <string>

#include "driver.h"
#include "hw2.h"


Driver::Driver(QObject *parent): QObject(parent){

}

/*!
 * \brief Driver::run runs the driver for HW2
 */
void Driver::run(){
    HW2::HW2Driver hw2;

    //start by loading a mesh
    QMessageBox::information(0, "HW2", "Please choose an obj mesh");

    QFileDialog fileDialog(0, "Open Mesh", "./", "Waveform Object (*.obj)");
    QString filename("");

    if(fileDialog.exec())
        filename = fileDialog.selectedFiles().first();

    if(filename.isNull() || filename.isEmpty()){
        std::cerr<<"Must specify a file, exiting"<<std::endl;
        QMessageBox::critical(0, "Error", "You must specify a valid file");
        emit finished();
        return;
    }

    if(!hw2.loadMesh(filename.toStdString())){
        QMessageBox::critical(0,"Error", "Failed to load mesh!\nExiting");
        emit finished();
        return;
    }

    std::string output = "";
    std::string totalOut = "";

    //get the genus
    int genus = QInputDialog::getInt(0 , "Mesh Genus", "Enter the genus of the mesh");

    //get the euler number using the mesh
    int euler = hw2.calcEulerFromMesh();
    output = "The euler number found using v+f-e is " + std::to_string(static_cast<long long>(euler)) + "\n";
    totalOut += output;
    std::cout<<output;

    //get the euler number using the
    euler = hw2.calcEulerFromGenus(genus);
    output = "The euler number found using 2-2g is " + std::to_string(static_cast<long long>(euler)) + "\n";
    totalOut += output;
    std::cout<<output;

    //get the gaussian curvature from the mesh
    double curve = 0;
    try{
        curve = hw2.calcGausCurveFromMesh();
    }catch(std::runtime_error &e){
        QMessageBox::critical(0, "Error!", QString(e.what()));
        qFatal(e.what());
    }
    output = "The Gaussian curvature using left side of Gauss-Bonnet theorem is " + std::to_string(static_cast<long double>(curve)) + "\n";
    totalOut += output;
    std::cout<<output;

    //get the gaussian curvature from the euler number
    curve = hw2.calcGausCurveFromEuler(euler);
    output = "The Gaussian curvate using the right side of Gauss-Bonnet theorem is "+ std::to_string(static_cast<long double>(curve)) + "\n";
    totalOut += output;
    std::cout<<output;

    //display results
    QMessageBox::information(0, "Results", QString(totalOut.c_str()));

    emit finished();
}


int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    Driver driver;

    //setup startup and shutdown of driver for application
    QObject::connect(&driver,SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, &driver, SLOT(run()));

    //start application
    return a.exec();
}

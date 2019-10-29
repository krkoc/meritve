#include "calibration.h"
#include "QHBoxLayout"
#include <QDebug>

Calibration::Calibration(QWidget *parent) : QWidget(parent)
{
    calibrateButton = new QPushButton(this);
    setWindowModality(Qt::ApplicationModal);
    calibrateButton->setText("sprejmi vrednost");
    realValue = new QDoubleSpinBox(this);
    realValue->setDecimals(4);
    QHBoxLayout *topLeftLayout = new QHBoxLayout(this);
    topLeftLayout->addWidget(calibrateButton);
    topLeftLayout->addWidget(realValue);

   // connect(calibrateButton, SIGNAL(clicked()), this, SIGNAL(sendCalibrationParameter()));
    
    //emit sendCalibrationParameter();

    connect(this->calibrateButton,SIGNAL(clicked()),this, SLOT(hide()));
    connect(this->calibrateButton,SIGNAL(clicked()),this, SLOT(enterCalibrationMode())  );
}



Calibration::~Calibration()
{
    calibrationMode=0; //disable calibration mode
    qDebug()<<"disabling calibration mode";

}

void Calibration::enterCalibrationMode(){
    calibrationMode=1;
    qDebug()<<"entering calibration mode...";
}


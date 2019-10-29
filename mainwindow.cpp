#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>

#include "gpiothread.h"


QT_USE_NAMESPACE
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //make spinBoxL of 1
    comCounter=0;
    ui->setupUi(this);
    this->setWindowTitle("CNCK");
    calibWindow= new Calibration();
    filenameL="";
    timer = new QTimer;
    transmitTimer=new QTimer;
    transmitTimer->setSingleShot(true);
    transmitTimer->setInterval(220);
    timer->start(2000);

    gpiothread = new GPIOThread();
    gpiothread->start();

    ui->lineEditL->setDisabled(true);
    row_countL=1;
    column_count=2;
    ui->tableWidgetL->setRowCount(row_countL);
    ui->tableWidgetL->setColumnCount(column_count);

    table_widthL=ui->tableWidgetL->width();
    table_widthR=ui->tableWidgetR->width();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        ui->comComboL->addItem(info.portName());
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        ui->comComboR->addItem(info.portName());

    connect(ui->zajemiButtonL, SIGNAL(clicked()),this, SLOT(transaction()));

    connect(ui->zajemiButtonL, SIGNAL(clicked()),this, SLOT(transaction()));
    connect(gpiothread,SIGNAL(simulateButton()),this,SLOT(transaction()));


    connect(&threadL, SIGNAL(response(QString,QString)), this, SLOT(showResponse(QString,QString)));
    connect(&threadR, SIGNAL(response(QString,QString)), this, SLOT(showResponse(QString,QString)));

    connect(ui->calibrationButton_L, SIGNAL(clicked()), this, SLOT(calibrationDialog1()));
    connect(ui->calibrationButton_R, SIGNAL(clicked()), this, SLOT(calibrationDialog2()));

    connect(calibWindow->calibrateButton, SIGNAL(clicked()),this,SLOT(receiveCalibrationParameter()) );
    connect(ui->pushIzberiDat_L, SIGNAL(clicked()),this,SLOT(getFilenameL()) );

    connect(ui->lineEditL,SIGNAL(textChanged(QString)), this, SLOT(changeFileNameL(QString)));
    connect(ui->spinBoxL,SIGNAL(valueChanged(int)),this,SLOT(updateComMap()));
    connect(ui->spinBoxR,SIGNAL(valueChanged(int)),this,SLOT(updateComMap()));

    //connect(timer,SIGNAL(timeout()),this, SLOT(getThisValue()) );

    // connect(&thread, SIGNAL(error(QString)),this, SLOT(processError(QString)));
    // connect(&thread, SIGNAL(timeout(QString)),this, SLOT(processTimeout(QString)));
    trenutno=0;
    this->updateComMap();
}

void MainWindow::calibrationDialog1()
{
    calibWindow->show();
    calibWindow->calibrationMode=1;
    this->side="left";
}

void MainWindow::calibrationDialog2()
{
    calibWindow->show();
    calibWindow->calibrationMode=1;
    this->side="right";
}


void MainWindow::receiveCalibrationParameter()
{
    //read real
    qDebug()<<"entering rcvval";
    if (this->side=="left"){
        threadL.transaction(ui->comComboL->currentText(),200,"?\r");
        qDebug()<<"receive calib param for left channel";
    }
    if (this->side=="right"){
        threadR.transaction(ui->comComboR->currentText(),200,"?\r");;
        qDebug()<<"receive calib param for right channel";
    }
}

void MainWindow::getThisValue()
{
    trenutno=1;
    calibWindow->calibrationMode=0;
    querryThread(map[comCounter]);
    //qDebug()<<"com counter in getthis"<<comCounter;
}




void MainWindow::updateComMap(){
    map.clear();
    comCounter=0;
    for (int i=0 ; i<ui->spinBoxL->value();i++){
        map[i]=1;
        row_countL=i+1;
    }

    //add spinBoxR of 2
    for (int i=0 ; i<ui->spinBoxR->value();i++){
        map[i+ui->spinBoxL->value()]=2;
        row_countR=i+1;
    }
    qDebug()<<map;
    resizeTable(ui->tableWidgetL, row_countL, table_widthL);
    resizeTable(ui->tableWidgetR, row_countR, table_widthR);
    resizeMeasurementTable(ui->meritveTable, row_countL+row_countR);

}



void MainWindow::transaction()
{
    QMap<double, double> toleranceMap;

    disconnect(ui->zajemiButtonL, SIGNAL(clicked()),this, SLOT(transaction()));
    timer->stop();
    trenutno=0;
    calibWindow->calibrationMode=0;
    for (comCounter=1;comCounter < 3 ;comCounter++) {
        transmitTimer->start();
        querryThread(comCounter);
        while(transmitTimer->remainingTime()>0);
        transmitTimer->stop();

    }
    //if (comCounter>=(map.size()))
    //    comCounter=0;
}

void MainWindow::querryThread(int port){

    if (calibWindow->calibrationMode==0){
        if (port==1){
            threadL.transaction(ui->comComboL->currentText(),200,"?\r");

            // qDebug()<<"calling threadL";
        }
        else if (port==2){
            threadR.transaction(ui->comComboR->currentText(),200,"?\r");
            //qDebug()<<"calling threadR";
        }
    }
}



void MainWindow::showResponse(const QString &s,QString port)
{
    double value;
    int position=-1;
    if (port== ui->comComboL->currentText()) {

        if (calibWindow->calibrationMode){
            calibWindow->correctionL =-s.toDouble()+calibWindow->realValue->value();
            qDebug()<<"LEFT show response - raw: "<<s<<"correction: "<<calibWindow->correctionL;
        }
        else if (!calibWindow->calibrationMode){
            //value=(-s.toDouble()+calibWindow->correctionL);
            value=(s.toDouble()+calibWindow->correctionL);
            qDebug()<<"raw left: "<<s<<" correctionL: "<<calibWindow->correctionL;
            //catch rules
            ui->plainTextEditL->appendPlainText(QString::number(value));
            //find position
            position = scanTable(value, ui->tableWidgetL);
            qDebug()<<"position: "<<position;

            //leak... fix this later
            item = new QTableWidgetItem;
            item->setText(QString::number(value));
            qDebug()<<"set text "<<item->text();
            //write to position
            if (position >=0){
                ui->meritveTable->setItem(0,position,item);
                position=-1;
            }
        }
    }

    if (port    == ui->comComboR->currentText()) {

        if (calibWindow->calibrationMode){
            calibWindow->correctionR =-s.toDouble()+calibWindow->realValue->value();
            qDebug()<<"right show response - raw: "<<s<<"correction: "<<calibWindow->correctionR;
        }
        else if (!calibWindow->calibrationMode){
            //value=(-s.toDouble()+calibWindow->correctionR);
            value=(s.toDouble()+calibWindow->correctionR);
            //catch rules
            ui->plainTextEditL->appendPlainText(QString::number(value));
            position = scanTable(value, ui->tableWidgetR);
            item = new QTableWidgetItem;
            item->setText(QString::number(value));
            qDebug()<<"set text "<<item->text();
            //write to position
            if (position >=0){
                ui->meritveTable->setItem(0,row_countL+position,item);
                position=-1;
            }
            //find position
            //write to position
        }
    }



    connect(ui->zajemiButtonL, SIGNAL(clicked()),this, SLOT(transaction()));
}





void MainWindow::processError(const QString &s)
{
    // setControlsEnabled(true);
    // statusLabel->setText(tr("Status: Not running, %1.").arg(s));
    // trafficLabel->setText(tr("No traffic."));
}


void MainWindow::processTimeout(const QString &s)

{
    //setControlsEnabled(true);
    //statusLabel->setText(tr("Status: Running, %1.").arg(s));
    //trafficLabel->setText(tr("No traffic."));
}


MainWindow::~MainWindow()
{
    //close all files before quitting
    delete ui;
}

void MainWindow::changeFileNameL(QString filename)
{
    //this gets called every time the file edit box changes
    //close opened files
    fileL.close();
    fileL.setFileName(filename);
    //open new file
    if (fileL.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug()<<"file"<< filenameL <<"opened";
    }
}


void MainWindow::getFilenameL()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    filenameL=dialog.getSaveFileName(this,
                                     tr("Odri datoteko za pisanje"), "", tr("CSV (*.csv)") );
    ui->lineEditL->setText(filenameL);
}


void MainWindow::resizeTable(QTableWidget * table, int rowCount, int tableWidth)
{
    table->setRowCount(rowCount);
    table->setColumnCount(column_count);
    table->resize(tableWidth,(rowCount+1)*table->rowHeight(0));
}


bool MainWindow::isWithinTolerance(double value, double lowLimit, double highLimit){
    return ((value >= lowLimit)&&(value<=highLimit)  );
}

int MainWindow::scanTable(double value, QTableWidget *table){
    qDebug()<<"row count"<<table->rowCount();
    qDebug()<<"column count"<<table->columnCount();
    int i=-1;
    for (i=0; i < table->rowCount();i++)
        if (table->item(i,0) && (table->item(i,1)) ){
            //qDebug()<<"io: "<<table->item(i,0)->text().toDouble();
            if ((value > table->item(i,0)->text().toDouble()) && (value < table->item(i,1)->text().toDouble())){
                qDebug()<<"i"<<i;
                return i;
            }
            // qDebug()<<"item: "<<table->item(j,i)->text() <<"i: "<<i<<" j: "<<j;
        }
    qDebug()<<"i_exit"<<i;
    return -1;

}


void MainWindow::resizeMeasurementTable(QTableWidget *table, int columnCount){
    //creates and updates the measurement table
    table->setColumnCount(columnCount);
    table->setRowCount(1);
    table->resize(table->columnWidth(1)*columnCount ,(2)*table->rowHeight(0));
}


//void MainWindow::buildMapFromTable(){//call every time the table is updated

//}

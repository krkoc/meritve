#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "masterthread.h"
#include "calibration.h"
<<<<<<< HEAD
=======
#include "gpiothread.h"
>>>>>>> version on raspbi w/ gpio
#include <QFile>
#include <QTimer>
#include <QTableWidget>
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void transaction();
    void resizeTable(QTableWidget * table, int rowCount, int tableWidth);
    void resizeMeasurementTable(QTableWidget *table, int columnCount);
    void showResponse(const QString &s,QString port);
    //void showResponseR(const QString &s);

    void getThisValue();
    void processError(const QString &s);
    void processTimeout(const QString &s);
    void calibrationDialog1();
    void calibrationDialog2();
    void receiveCalibrationParameter();
    void getFilenameL();
    void changeFileNameL(QString filename);
    void updateComMap();

private:
    int row_countL, column_count, table_widthL;
    int row_countR, table_widthR;


    void querryThread(int port);
<<<<<<< HEAD
=======
    GPIOThread *gpiothread;
>>>>>>> version on raspbi w/ gpio
    Ui::MainWindow *ui;
    MasterThread threadL, threadR;
    Calibration *calibWindow;
    QString filenameL, filenameR;
    QFile fileL,fileR;
    QMap<int, int> map;
    int comCounter;
    QString side;
    int port;
    QTimer *timer,*transmitTimer;
    int trenutno;
    bool isWithinTolerance(double value, double lowLimit, double highLimit);
    int scanTable(double value,  QTableWidget *table);
    void buildMapFromTable();
    QTableWidgetItem  *item;

};

#endif // MAINWINDOW_H

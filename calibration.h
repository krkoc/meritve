#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>
#include <QPushButton>
#include <QDoubleSpinBox>

class Calibration : public QWidget
{
    Q_OBJECT
public:
    explicit Calibration(QWidget *parent = 0);
    ~Calibration();
    QDoubleSpinBox * realValue;
    double correctionL, correctionR;
    QString side;
    bool calibrationMode;
    int calibrationPort;
    QPushButton * calibrateButton;

signals:
    void sendCalibrationParameter();

public slots:
    void enterCalibrationMode();
private:

};

#endif // CALIBRATION_H

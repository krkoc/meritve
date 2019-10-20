#ifndef GPIOTHREAD_H
#define GPIOTHREAD_H
#include <QtCore>
#include "mmapGpio.h"

class GPIOThread:public QThread
{
   Q_OBJECT


public:
    mmapGpio *rpiGpio;
    GPIOThread();
    void run();

signals:
    void simulateButton(void);
};

#endif // GPIOTHREAD_H

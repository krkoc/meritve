#include "gpiothread.h"

GPIOThread::GPIOThread()
{
    rpiGpio = new mmapGpio(); // instantiate mmapGpio object
    rpiGpio->setPinDir(17,mmapGpio::INPUT);
}

void GPIOThread::run()
{
    while(true)
    {
        unsigned int pinVal;
        pinVal = rpiGpio->readPin(17);
        qDebug()<<"pinval: "<<pinVal;
       if (pinVal == 1)
               simulateButton();
        this->sleep(1);
    }


}

#include "gpiothread.h"

GPIOThread::GPIOThread()
{
#if RPI
    rpiGpio = new mmapGpio(); // instantiate mmapGpio object
    rpiGpio->setPinDir(17,mmapGpio::INPUT);
#endif
}

void GPIOThread::run()
{
#if RPI
    while(true)
    {
        unsigned int pinVal;
        pinVal = rpiGpio->readPin(17);
        qDebug()<<"pinval: "<<pinVal;
       if (pinVal == 1 && debounce){
            simulateButton();
            debounce = false;
        }
       if (pinVal == 0){
        debounce = true;
       }
        this->sleep(1);
    }
#endif
}

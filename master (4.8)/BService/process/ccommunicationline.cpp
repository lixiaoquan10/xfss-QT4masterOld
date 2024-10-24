#include "ccommunicationline.h"
#include "cserialport.h"

CCommunicationLine::CCommunicationLine()
{
    pSerialPort = NULL;
    nPort = 1;
    recentRespondSerialNumber = -1;
    recentReceiveSerialNumber = -1;
}

void CCommunicationLine::destroy()
{
    delete pSerialPort;
}

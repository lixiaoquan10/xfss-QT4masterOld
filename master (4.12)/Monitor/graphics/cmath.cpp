#include "cglobal.h"


double getAngle360(const double angle)
{
    int multiplying=angle/360;
    double tmp=(multiplying==0)?angle:(angle-multiplying*360);
    if(tmp<0){
        tmp=360+tmp;
    }
    return tmp;
}



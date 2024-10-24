#include "cmath.h"
#include "math.h"
#include "dm/clayer.h"
#include "dm/cdm.h"
#include "dm/ccontroller.h"
#include "cglobal.h"

#include <QPointF>
#include <QLineF>
#include <QHash>
#include <QSet>
#include <QDebug>


double getAngle360(const double angle)
{
    int multiplying=angle/360;
    double tmp=(multiplying==0)?angle:(angle-multiplying*360);
    if(tmp<0){
        tmp=360+tmp;
    }
    return tmp;
}



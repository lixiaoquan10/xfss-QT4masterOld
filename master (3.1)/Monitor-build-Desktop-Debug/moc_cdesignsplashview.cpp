/****************************************************************************
** Meta object code from reading C++ file 'cdesignsplashview.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/widget/cdesignsplashview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cdesignsplashview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDesignSplashView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      43,   18,   18,   18, 0x0a,
      66,   60,   18,   18, 0x0a,
      96,   60,   18,   18, 0x0a,
     131,  125,   18,   18, 0x0a,
     161,   18,   18,   18, 0x0a,
     176,   18,   18,   18, 0x0a,
     193,  125,   18,   18, 0x0a,
     217,  125,   18,   18, 0x0a,
     239,   18,   18,   18, 0x0a,
     254,   18,   18,   18, 0x0a,
     272,   18,   18,   18, 0x0a,
     288,   18,   18,   18, 0x0a,
     305,   18,   18,   18, 0x0a,
     317,   18,   18,   18, 0x0a,
     330,   18,   18,   18, 0x0a,
     349,   18,   18,   18, 0x0a,
     369,   18,   18,   18, 0x0a,
     387,   18,   18,   18, 0x0a,
     405,   18,   18,   18, 0x0a,
     420,   18,   18,   18, 0x0a,
     437,   18,   18,   18, 0x0a,
     454,   18,   18,   18, 0x0a,
     470,   18,   18,   18, 0x0a,
     492,   18,   18,   18, 0x0a,
     513,   18,   18,   18, 0x0a,
     536,   18,   18,   18, 0x0a,
     558,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CDesignSplashView[] = {
    "CDesignSplashView\0\0slot_setPrimitiveItem()\0"
    "setMainPicture()\0event\0"
    "mousePressEvent(QMouseEvent*)\0"
    "mouseMoveEvent(QMouseEvent*)\0value\0"
    "slot_spliderValueChanged(int)\0"
    "slot_enlarge()\0slot_reduction()\0"
    "onHorizontalScroll(int)\0onVerticalScroll(int)\0"
    "slot_Lighten()\0slot_CloseLight()\0"
    "slot_AllClose()\0slot_AllBright()\0"
    "slot_Left()\0slot_Right()\0slot_DefaultLeft()\0"
    "slot_DefaultRight()\0slot_DefaultAll()\0"
    "slot_DefaultOff()\0slot_Reverse()\0"
    "slot_NoReverse()\0slot_StartTest()\0"
    "slot_StopTest()\0slot_StartEmergency()\0"
    "slot_StopEmergency()\0slot_CloseBatteryOut()\0"
    "slot_BatteryOutKeep()\0slot_DeviceInfo()\0"
};

void CDesignSplashView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDesignSplashView *_t = static_cast<CDesignSplashView *>(_o);
        switch (_id) {
        case 0: _t->slot_setPrimitiveItem(); break;
        case 1: _t->setMainPicture(); break;
        case 2: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 4: _t->slot_spliderValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->slot_enlarge(); break;
        case 6: _t->slot_reduction(); break;
        case 7: _t->onHorizontalScroll((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->onVerticalScroll((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->slot_Lighten(); break;
        case 10: _t->slot_CloseLight(); break;
        case 11: _t->slot_AllClose(); break;
        case 12: _t->slot_AllBright(); break;
        case 13: _t->slot_Left(); break;
        case 14: _t->slot_Right(); break;
        case 15: _t->slot_DefaultLeft(); break;
        case 16: _t->slot_DefaultRight(); break;
        case 17: _t->slot_DefaultAll(); break;
        case 18: _t->slot_DefaultOff(); break;
        case 19: _t->slot_Reverse(); break;
        case 20: _t->slot_NoReverse(); break;
        case 21: _t->slot_StartTest(); break;
        case 22: _t->slot_StopTest(); break;
        case 23: _t->slot_StartEmergency(); break;
        case 24: _t->slot_StopEmergency(); break;
        case 25: _t->slot_CloseBatteryOut(); break;
        case 26: _t->slot_BatteryOutKeep(); break;
        case 27: _t->slot_DeviceInfo(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDesignSplashView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDesignSplashView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CDesignSplashView,
      qt_meta_data_CDesignSplashView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDesignSplashView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDesignSplashView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDesignSplashView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDesignSplashView))
        return static_cast<void*>(const_cast< CDesignSplashView*>(this));
    return QWidget::qt_metacast(_clname);
}

int CDesignSplashView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

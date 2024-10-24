/****************************************************************************
** Meta object code from reading C++ file 'cprocessdistribution.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BService/process/cprocessdistribution.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cprocessdistribution.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CProcessDistribution[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       1,   49, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x08,
      32,   21,   21,   21, 0x08,
     108,   44,   21,   21, 0x0a,
     193,  140,   21,   21, 0x2a,
     260,  221,   21,   21, 0x2a,
     311,  284,   21,   21, 0x2a,
     338,  331,   21,   21, 0x2a,

 // enums: name, flags, count, data
     354, 0x1,    6,   53,

 // enum data: key, value
     365, uint(CProcessDistribution::NoneDirection),
     379, uint(CProcessDistribution::NoneFlashDirection),
     398, uint(CProcessDistribution::FlashDirection),
     413, uint(CProcessDistribution::LeftDirection),
     427, uint(CProcessDistribution::RightDirection),
     442, uint(CProcessDistribution::PeopleDirection),

       0        // eod
};

static const char qt_meta_stringdata_CProcessDistribution[] = {
    "CProcessDistribution\0\0timeOut()\0"
    "regeditOK()\0"
    "enable,distributionAddress,loopAddress,deviceAddress,canportAdd\0"
    "emergency(bool,int,int,int,int)\0"
    "enable,distributionAddress,loopAddress,deviceAddress\0"
    "emergency(bool,int,int,int)\0"
    "enable,distributionAddress,loopAddress\0"
    "emergency(bool,int,int)\0"
    "enable,distributionAddress\0"
    "emergency(bool,int)\0enable\0emergency(bool)\0"
    "Directions\0NoneDirection\0NoneFlashDirection\0"
    "FlashDirection\0LeftDirection\0"
    "RightDirection\0PeopleDirection\0"
};

void CProcessDistribution::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CProcessDistribution *_t = static_cast<CProcessDistribution *>(_o);
        switch (_id) {
        case 0: _t->timeOut(); break;
        case 1: _t->regeditOK(); break;
        case 2: _t->emergency((*reinterpret_cast< const bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 3: _t->emergency((*reinterpret_cast< const bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 4: _t->emergency((*reinterpret_cast< const bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->emergency((*reinterpret_cast< const bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->emergency((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CProcessDistribution::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CProcessDistribution::staticMetaObject = {
    { &CProcessBase::staticMetaObject, qt_meta_stringdata_CProcessDistribution,
      qt_meta_data_CProcessDistribution, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CProcessDistribution::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CProcessDistribution::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CProcessDistribution::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CProcessDistribution))
        return static_cast<void*>(const_cast< CProcessDistribution*>(this));
    return CProcessBase::qt_metacast(_clname);
}

int CProcessDistribution::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CProcessBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

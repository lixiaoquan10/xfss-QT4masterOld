/****************************************************************************
** Meta object code from reading C++ file 'cdevice.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/dm/cdevice.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cdevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDevice[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,    9,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      74,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CDevice[] = {
    "CDevice\0\0oldSecondCode,newSecondCode\0"
    "secondCodeChanged(QString&,QString&)\0"
    "slot_finished()\0"
};

void CDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDevice *_t = static_cast<CDevice *>(_o);
        switch (_id) {
        case 0: _t->secondCodeChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->slot_finished(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDevice::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDevice::staticMetaObject = {
    { &CObject::staticMetaObject, qt_meta_stringdata_CDevice,
      qt_meta_data_CDevice, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDevice::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDevice))
        return static_cast<void*>(const_cast< CDevice*>(this));
    return CObject::qt_metacast(_clname);
}

int CDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CDevice::secondCodeChanged(QString & _t1, QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'cserialport.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BService/process/cserialport.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cserialport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSerialPort[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   13,   12,   12, 0x05,
      77,   58,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     129,  115,   12,   12, 0x0a,
     159,   12,   12,   12, 0x0a,
     174,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSerialPort[] = {
    "CSerialPort\0\0name,data\0"
    "sig_ReciveData(QString,QByteArray)\0"
    "name,nSerialNumber\0"
    "updataRecentSerialNumber(QString,int)\0"
    "nSerialNumber\0handleConfirmationPacket(int)\0"
    "doReciveData()\0checkTimeOut()\0"
};

void CSerialPort::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CSerialPort *_t = static_cast<CSerialPort *>(_o);
        switch (_id) {
        case 0: _t->sig_ReciveData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 1: _t->updataRecentSerialNumber((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->handleConfirmationPacket((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->doReciveData(); break;
        case 4: _t->checkTimeOut(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CSerialPort::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CSerialPort::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CSerialPort,
      qt_meta_data_CSerialPort, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSerialPort::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSerialPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSerialPort::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSerialPort))
        return static_cast<void*>(const_cast< CSerialPort*>(this));
    return QObject::qt_metacast(_clname);
}

int CSerialPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CSerialPort::sig_ReciveData(const QString & _t1, const QByteArray & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CSerialPort::updataRecentSerialNumber(const QString & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'ccommunicationmanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BService/process/ccommunicationmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccommunicationmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCommunicationManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   23,   22,   22, 0x05,
      98,   61,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
     171,  161,   22,   22, 0x0a,
     246,  209,  204,   22, 0x0a,
     310,  209,  204,   22, 0x0a,
     372,   22,   22,   22, 0x08,
     389,   22,   22,   22, 0x08,
     409,   22,   22,   22, 0x08,
     431,   22,   22,   22, 0x08,
     452,   22,   22,   22, 0x08,
     473,   22,   22,   22, 0x08,
     493,   22,   22,   22, 0x08,
     512,   22,   22,   22, 0x08,
     547,  528,   22,   22, 0x08,
     585,   22,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CCommunicationManager[] = {
    "CCommunicationManager\0\0nSerialNumber\0"
    "sig_ReceivedPacket(int)\0"
    "lineName,infoType,controlDomain,data\0"
    "sig_ReciveData(QString,int,QHash<QString,QVariant>,QByteArray)\0"
    "name,data\0doReciveData(QString,QByteArray)\0"
    "bool\0lineName,infotype,controlDomain,data\0"
    "sendCommandData(QString,int,QHash<QString,QVariant>,QByteArray)\0"
    "sendPrintData(QString,int,QHash<QString,QVariant>,QByteArray)\0"
    "handleSendData()\0handleSendLEDData()\0"
    "handleSendPowerData()\0handleSendCan1Data()\0"
    "handleSendCan2Data()\0handleSendFasData()\0"
    "handleReciveData()\0handleCanData()\0"
    "name,nSerialNumber\0"
    "updataRecentSerialNumber(QString,int)\0"
    "handlePrinterData()\0"
};

void CCommunicationManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCommunicationManager *_t = static_cast<CCommunicationManager *>(_o);
        switch (_id) {
        case 0: _t->sig_ReceivedPacket((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sig_ReciveData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QHash<QString,QVariant>(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 2: _t->doReciveData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 3: { bool _r = _t->sendCommandData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QHash<QString,QVariant>(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = _t->sendPrintData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QHash<QString,QVariant>(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: _t->handleSendData(); break;
        case 6: _t->handleSendLEDData(); break;
        case 7: _t->handleSendPowerData(); break;
        case 8: _t->handleSendCan1Data(); break;
        case 9: _t->handleSendCan2Data(); break;
        case 10: _t->handleSendFasData(); break;
        case 11: _t->handleReciveData(); break;
        case 12: _t->handleCanData(); break;
        case 13: _t->updataRecentSerialNumber((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->handlePrinterData(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCommunicationManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCommunicationManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CCommunicationManager,
      qt_meta_data_CCommunicationManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCommunicationManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCommunicationManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCommunicationManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCommunicationManager))
        return static_cast<void*>(const_cast< CCommunicationManager*>(this));
    return QObject::qt_metacast(_clname);
}

int CCommunicationManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void CCommunicationManager::sig_ReceivedPacket(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CCommunicationManager::sig_ReciveData(const QString & _t1, const int & _t2, const QHash<QString,QVariant> & _t3, const QByteArray & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

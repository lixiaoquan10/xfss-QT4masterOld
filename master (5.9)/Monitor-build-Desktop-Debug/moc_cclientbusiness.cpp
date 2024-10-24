/****************************************************************************
** Meta object code from reading C++ file 'cclientbusiness.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/business/cclientbusiness.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cclientbusiness.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CClientBusiness[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   17,   16,   16, 0x05,
      93,   73,   16,   16, 0x05,
     128,   73,   16,   16, 0x05,
     188,  168,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     221,   16,   16,   16, 0x0a,
     242,   16,   16,   16, 0x0a,
     258,   16,   16,   16, 0x0a,
     316,  279,   16,   16, 0x0a,
     385,   16,   16,   16, 0x08,
     409,  395,   16,   16, 0x08,
     435,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CClientBusiness[] = {
    "CClientBusiness\0\0type,information\0"
    "notifyInformation(int,QList<QVariant>)\0"
    "commandType,msgData\0"
    "commandResult(int,CMsgStructBase*)\0"
    "requestVerifyAdmin(int,CMsgStructBase*)\0"
    "statusInfo,type,opr\0"
    "statusRecord(StatusInfo,int,int)\0"
    "slotCheckLoginInfo()\0mainPowerStop()\0"
    "slot_saveLoginData()\0"
    "object,status,oldValue,newValue,time\0"
    "objectStatusChanged(CObject*,short,unsigned char,unsigned char,uint)\0"
    "timeOut()\0nMsgType,data\0"
    "recvData(int,QByteArray&)\0"
    "slot_ledStatusSend()\0"
};

void CClientBusiness::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CClientBusiness *_t = static_cast<CClientBusiness *>(_o);
        switch (_id) {
        case 0: _t->notifyInformation((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QList<QVariant>(*)>(_a[2]))); break;
        case 1: _t->commandResult((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< CMsgStructBase*(*)>(_a[2]))); break;
        case 2: _t->requestVerifyAdmin((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< CMsgStructBase*(*)>(_a[2]))); break;
        case 3: _t->statusRecord((*reinterpret_cast< const StatusInfo(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->slotCheckLoginInfo(); break;
        case 5: _t->mainPowerStop(); break;
        case 6: _t->slot_saveLoginData(); break;
        case 7: _t->objectStatusChanged((*reinterpret_cast< CObject*(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2])),(*reinterpret_cast< unsigned char(*)>(_a[3])),(*reinterpret_cast< unsigned char(*)>(_a[4])),(*reinterpret_cast< uint(*)>(_a[5]))); break;
        case 8: _t->timeOut(); break;
        case 9: _t->recvData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 10: _t->slot_ledStatusSend(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CClientBusiness::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CClientBusiness::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CClientBusiness,
      qt_meta_data_CClientBusiness, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CClientBusiness::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CClientBusiness::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CClientBusiness::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CClientBusiness))
        return static_cast<void*>(const_cast< CClientBusiness*>(this));
    return QObject::qt_metacast(_clname);
}

int CClientBusiness::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void CClientBusiness::notifyInformation(int _t1, const QList<QVariant> & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CClientBusiness::commandResult(int _t1, CMsgStructBase * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CClientBusiness::requestVerifyAdmin(int _t1, CMsgStructBase * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CClientBusiness::statusRecord(const StatusInfo & _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE

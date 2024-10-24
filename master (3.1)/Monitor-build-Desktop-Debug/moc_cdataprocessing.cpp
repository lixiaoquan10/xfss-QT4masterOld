/****************************************************************************
** Meta object code from reading C++ file 'cdataprocessing.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/business/cdataprocessing.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cdataprocessing.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDataProcessing[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,   17,   16,   16, 0x0a,
      90,   70,   16,   16, 0x0a,
     129,   70,   16,   16, 0x0a,
     186,  166,   16,   16, 0x0a,
     227,   16,   16,   16, 0x0a,
     245,   16,   16,   16, 0x0a,
     266,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CDataProcessing[] = {
    "CDataProcessing\0\0type,data\0"
    "slotNotifyInformation(int,QList<QVariant>)\0"
    "commandType,msgData\0"
    "slotCommandResult(int,CMsgStructBase*)\0"
    "slotVerifyAdmin(int,CMsgStructBase*)\0"
    "statusInfo,type,opr\0"
    "slotStateInformation(StatusInfo,int,int)\0"
    "slotChangeColor()\0slotcheckLoginInfo()\0"
    "slot_usetimeCheck()\0"
};

void CDataProcessing::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDataProcessing *_t = static_cast<CDataProcessing *>(_o);
        switch (_id) {
        case 0: _t->slotNotifyInformation((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QList<QVariant>(*)>(_a[2]))); break;
        case 1: _t->slotCommandResult((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< CMsgStructBase*(*)>(_a[2]))); break;
        case 2: _t->slotVerifyAdmin((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< CMsgStructBase*(*)>(_a[2]))); break;
        case 3: _t->slotStateInformation((*reinterpret_cast< const StatusInfo(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->slotChangeColor(); break;
        case 5: _t->slotcheckLoginInfo(); break;
        case 6: _t->slot_usetimeCheck(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDataProcessing::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDataProcessing::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CDataProcessing,
      qt_meta_data_CDataProcessing, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDataProcessing::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDataProcessing::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDataProcessing::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDataProcessing))
        return static_cast<void*>(const_cast< CDataProcessing*>(this));
    return QObject::qt_metacast(_clname);
}

int CDataProcessing::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

/****************************************************************************
** Meta object code from reading C++ file 'cadapterobject.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BService/cadapterobject.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cadapterobject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAdapterObject[] = {

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
      30,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,   16,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CAdapterObject[] = {
    "CAdapterObject\0\0nMsgType,data\0"
    "sigParsePacket(int,QByteArray)\0"
    "slotParsePacket(int,QByteArray)\0"
};

void CAdapterObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAdapterObject *_t = static_cast<CAdapterObject *>(_o);
        switch (_id) {
        case 0: _t->sigParsePacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 1: _t->slotParsePacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAdapterObject::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAdapterObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAdapterObject,
      qt_meta_data_CAdapterObject, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAdapterObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAdapterObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAdapterObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAdapterObject))
        return static_cast<void*>(const_cast< CAdapterObject*>(this));
    return QObject::qt_metacast(_clname);
}

int CAdapterObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void CAdapterObject::sigParsePacket(int _t1, QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

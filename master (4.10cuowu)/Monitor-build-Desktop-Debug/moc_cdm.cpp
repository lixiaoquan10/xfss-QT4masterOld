/****************************************************************************
** Meta object code from reading C++ file 'cdm.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/dm/cdm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cdm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDM[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,    5,    4,    4, 0x05,
      34,    5,    4,    4, 0x05,
      69,   58,    4,    4, 0x05,
      92,   58,    4,    4, 0x05,
     155,  118,    4,    4, 0x05,

 // slots: signature, parameters, type, tag, flags
     234,  227,    4,    4, 0x09,
     303,  273,    4,    4, 0x09,
     375,  363,    4,    4, 0x09,
     447,  419,    4,    4, 0x09,
     506,  484,    4,    4, 0x09,
     540,    5,    4,    4, 0x09,
     565,    5,    4,    4, 0x09,
     598,  582,    4,    4, 0x09,
     620,    5,    4,    4, 0x29,
     644,  637,    4,    4, 0x09,
     679,  663,    4,    4, 0x09,
     706,  637,    4,    4, 0x29,
     734,  728,    4,    4, 0x09,
     772,  728,    4,    4, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CDM[] = {
    "CDM\0\0object\0addedObject(CObject*)\0"
    "removedObject(CObject*)\0keyId,type\0"
    "statusChanged(int,int)\0undefinedChanged(int,int)\0"
    "object,status,oldValue,newValue,time\0"
    "sigObjectStatusChanged(CObject*,short,unsigned char,unsigned char,uint"
    ")\0"
    "status\0editStatusChanged(CObject::EditStatus)\0"
    "status,oldValue,newValue,time\0"
    "objectStatusChanged(short,unsigned char,unsigned char,uint)\0"
    "type,object\0objectChanged(CObject::ObjectType,CObject*)\0"
    "oldSecondCode,newSecondCode\0"
    "secondCodeChanged(QString&,QString&)\0"
    "oldAddress,newAddress\0"
    "addressChanged(QString&,QString&)\0"
    "objectDestrory(CObject*)\0insert(CObject*)\0"
    "object,isDelete\0remove(CObject*,bool)\0"
    "remove(CObject*)\0parent\0addChild(CObject*)\0"
    "parent,isDelete\0removeChild(CObject*,bool)\0"
    "removeChild(CObject*)\0layer\0"
    "addNeedUpdateEvacuationLayer(CLayer*)\0"
    "removeNeedUpdateEvacuationLayer(CLayer*)\0"
};

void CDM::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDM *_t = static_cast<CDM *>(_o);
        switch (_id) {
        case 0: _t->addedObject((*reinterpret_cast< CObject*(*)>(_a[1]))); break;
        case 1: _t->removedObject((*reinterpret_cast< CObject*(*)>(_a[1]))); break;
        case 2: _t->statusChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->undefinedChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->sigObjectStatusChanged((*reinterpret_cast< CObject*(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2])),(*reinterpret_cast< unsigned char(*)>(_a[3])),(*reinterpret_cast< unsigned char(*)>(_a[4])),(*reinterpret_cast< uint(*)>(_a[5]))); break;
        case 5: _t->editStatusChanged((*reinterpret_cast< CObject::EditStatus(*)>(_a[1]))); break;
        case 6: _t->objectStatusChanged((*reinterpret_cast< short(*)>(_a[1])),(*reinterpret_cast< unsigned char(*)>(_a[2])),(*reinterpret_cast< unsigned char(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 7: _t->objectChanged((*reinterpret_cast< CObject::ObjectType(*)>(_a[1])),(*reinterpret_cast< CObject*(*)>(_a[2]))); break;
        case 8: _t->secondCodeChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: _t->addressChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: _t->objectDestrory((*reinterpret_cast< CObject*(*)>(_a[1]))); break;
        case 11: _t->insert((*reinterpret_cast< CObject*(*)>(_a[1]))); break;
        case 12: _t->remove((*reinterpret_cast< CObject*(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 13: _t->remove((*reinterpret_cast< CObject*(*)>(_a[1]))); break;
        case 14: _t->addChild((*reinterpret_cast< CObject*(*)>(_a[1]))); break;
        case 15: _t->removeChild((*reinterpret_cast< CObject*(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 16: _t->removeChild((*reinterpret_cast< CObject*(*)>(_a[1]))); break;
        case 17: _t->addNeedUpdateEvacuationLayer((*reinterpret_cast< CLayer*(*)>(_a[1]))); break;
        case 18: _t->removeNeedUpdateEvacuationLayer((*reinterpret_cast< CLayer*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDM::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDM::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CDM,
      qt_meta_data_CDM, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDM::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDM::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDM::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDM))
        return static_cast<void*>(const_cast< CDM*>(this));
    return QObject::qt_metacast(_clname);
}

int CDM::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void CDM::addedObject(CObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDM::removedObject(CObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CDM::statusChanged(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CDM::undefinedChanged(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CDM::sigObjectStatusChanged(CObject * _t1, short _t2, unsigned char _t3, unsigned char _t4, unsigned int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE

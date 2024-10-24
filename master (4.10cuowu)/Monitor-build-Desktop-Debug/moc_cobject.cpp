/****************************************************************************
** Meta object code from reading C++ file 'cobject.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/dm/cobject.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cobject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CObject[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       1,   39, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,    9,    8,    8, 0x05,
      85,   55,    8,    8, 0x05,
     168,  145,    8,    8, 0x05,
     223,  211,    8,    8, 0x05,
     274,  267,    8,    8, 0x05,

 // enums: name, flags, count, data
     293, 0x1,    4,   43,

 // enum data: key, value
     304, uint(CObject::ES_None),
     312, uint(CObject::ES_New),
     319, uint(CObject::ES_Modify),
     329, uint(CObject::ES_Deleted),

       0        // eod
};

static const char qt_meta_stringdata_CObject[] = {
    "CObject\0\0status\0editStatusChanged(CObject::EditStatus)\0"
    "status,oldValue,newValue,time\0"
    "objectStatusChanged(short,unsigned char,unsigned char,uint)\0"
    "name,oldValue,newValue\0"
    "valueChanged(QString&,QVariant&,QVariant&)\0"
    "type,object\0objectChanged(CObject::ObjectType,CObject*)\0"
    "object\0destrory(CObject*)\0EditStatus\0"
    "ES_None\0ES_New\0ES_Modify\0ES_Deleted\0"
};

void CObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CObject *_t = static_cast<CObject *>(_o);
        switch (_id) {
        case 0: _t->editStatusChanged((*reinterpret_cast< CObject::EditStatus(*)>(_a[1]))); break;
        case 1: _t->objectStatusChanged((*reinterpret_cast< short(*)>(_a[1])),(*reinterpret_cast< unsigned char(*)>(_a[2])),(*reinterpret_cast< unsigned char(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 2: _t->valueChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 3: _t->objectChanged((*reinterpret_cast< CObject::ObjectType(*)>(_a[1])),(*reinterpret_cast< CObject*(*)>(_a[2]))); break;
        case 4: _t->destrory((*reinterpret_cast< CObject*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CObject::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CObject,
      qt_meta_data_CObject, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CObject))
        return static_cast<void*>(const_cast< CObject*>(this));
    return QObject::qt_metacast(_clname);
}

int CObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CObject::editStatusChanged(CObject::EditStatus _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CObject::objectStatusChanged(short _t1, unsigned char _t2, unsigned char _t3, unsigned int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CObject::valueChanged(QString & _t1, QVariant & _t2, QVariant & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CObject::objectChanged(CObject::ObjectType _t1, CObject * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CObject::destrory(CObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE

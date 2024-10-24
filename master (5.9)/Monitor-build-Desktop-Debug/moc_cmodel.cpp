/****************************************************************************
** Meta object code from reading C++ file 'cmodel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/model/cmodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      31,    8,    7,    7, 0x09,
      86,   74,    7,    7, 0x09,
     160,  130,    7,    7, 0x09,
     227,  220,    7,    7, 0x09,
     273,  266,    7,    7, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CModel[] = {
    "CModel\0\0name,oldValue,newValue\0"
    "valueChanged(QString&,QVariant&,QVariant&)\0"
    "type,object\0objectChanged(CObject::ObjectType,CObject*)\0"
    "status,oldValue,newValue,time\0"
    "objectStatusChanged(short,unsigned char,unsigned char,uint)\0"
    "status\0editStatusChanged(CObject::EditStatus)\0"
    "object\0objectDestroy(CObject*)\0"
};

void CModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CModel *_t = static_cast<CModel *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 1: _t->objectChanged((*reinterpret_cast< CObject::ObjectType(*)>(_a[1])),(*reinterpret_cast< CObject*(*)>(_a[2]))); break;
        case 2: _t->objectStatusChanged((*reinterpret_cast< short(*)>(_a[1])),(*reinterpret_cast< unsigned char(*)>(_a[2])),(*reinterpret_cast< unsigned char(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 3: _t->editStatusChanged((*reinterpret_cast< CObject::EditStatus(*)>(_a[1]))); break;
        case 4: _t->objectDestroy((*reinterpret_cast< CObject*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CModel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_CModel,
      qt_meta_data_CModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CModel))
        return static_cast<void*>(const_cast< CModel*>(this));
    return QAbstractItemModel::qt_metacast(_clname);
}

int CModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

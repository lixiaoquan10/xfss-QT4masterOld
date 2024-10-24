/****************************************************************************
** Meta object code from reading C++ file 'ccontrollertreemodel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/model/ccontrollertreemodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccontrollertreemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CControllerTreeModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   22,   21,   21, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CControllerTreeModel[] = {
    "CControllerTreeModel\0\0object\0"
    "objectDestroy(CObject*)\0"
};

void CControllerTreeModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CControllerTreeModel *_t = static_cast<CControllerTreeModel *>(_o);
        switch (_id) {
        case 0: _t->objectDestroy((*reinterpret_cast< CObject*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CControllerTreeModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CControllerTreeModel::staticMetaObject = {
    { &CModel::staticMetaObject, qt_meta_stringdata_CControllerTreeModel,
      qt_meta_data_CControllerTreeModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CControllerTreeModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CControllerTreeModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CControllerTreeModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CControllerTreeModel))
        return static_cast<void*>(const_cast< CControllerTreeModel*>(this));
    return CModel::qt_metacast(_clname);
}

int CControllerTreeModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

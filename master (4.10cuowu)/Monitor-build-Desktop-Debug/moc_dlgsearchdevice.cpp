/****************************************************************************
** Meta object code from reading C++ file 'dlgsearchdevice.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/dialog/dlgsearchdevice.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgsearchdevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dlgSearchDevice[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      53,   47,   16,   16, 0x08,
      90,   47,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_dlgSearchDevice[] = {
    "dlgSearchDevice\0\0slot_on_dClicked(QModelIndex)\0"
    "index\0on_comboBox_currentIndexChanged(int)\0"
    "on_comboBox_2_currentIndexChanged(int)\0"
};

void dlgSearchDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dlgSearchDevice *_t = static_cast<dlgSearchDevice *>(_o);
        switch (_id) {
        case 0: _t->slot_on_dClicked((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 1: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_comboBox_2_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData dlgSearchDevice::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dlgSearchDevice::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_dlgSearchDevice,
      qt_meta_data_dlgSearchDevice, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dlgSearchDevice::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dlgSearchDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dlgSearchDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dlgSearchDevice))
        return static_cast<void*>(const_cast< dlgSearchDevice*>(this));
    return QDialog::qt_metacast(_clname);
}

int dlgSearchDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'dlgdevices.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/dialog/dlgdevices.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgdevices.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dlgDevices[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      29,   11,   11,   11, 0x08,
      45,   11,   11,   11, 0x08,
      67,   11,   11,   11, 0x08,
      88,   11,   11,   11, 0x08,
     111,   11,   11,   11, 0x08,
     133,   11,   11,   11, 0x08,
     150,   11,   11,   11, 0x08,
     176,  170,   11,   11, 0x08,
     210,   11,   11,   11, 0x08,
     254,  248,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_dlgDevices[] = {
    "dlgDevices\0\0slot_StartTest()\0"
    "slot_StopTest()\0slot_StartEmergency()\0"
    "slot_StopEmergency()\0slot_CloseBatteryOut()\0"
    "slot_BatteryOutKeep()\0slot_deviceUse()\0"
    "slot_deviceForbid()\0index\0"
    "on_tableView_clicked(QModelIndex)\0"
    "on_ReadBatteryStatus_Button_clicked()\0"
    "value\0on_pageScrollBar_valueChanged(int)\0"
};

void dlgDevices::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dlgDevices *_t = static_cast<dlgDevices *>(_o);
        switch (_id) {
        case 0: _t->slot_StartTest(); break;
        case 1: _t->slot_StopTest(); break;
        case 2: _t->slot_StartEmergency(); break;
        case 3: _t->slot_StopEmergency(); break;
        case 4: _t->slot_CloseBatteryOut(); break;
        case 5: _t->slot_BatteryOutKeep(); break;
        case 6: _t->slot_deviceUse(); break;
        case 7: _t->slot_deviceForbid(); break;
        case 8: _t->on_tableView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: _t->on_ReadBatteryStatus_Button_clicked(); break;
        case 10: _t->on_pageScrollBar_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData dlgDevices::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dlgDevices::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_dlgDevices,
      qt_meta_data_dlgDevices, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dlgDevices::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dlgDevices::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dlgDevices::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dlgDevices))
        return static_cast<void*>(const_cast< dlgDevices*>(this));
    return QDialog::qt_metacast(_clname);
}

int dlgDevices::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

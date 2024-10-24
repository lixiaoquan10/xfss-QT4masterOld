/****************************************************************************
** Meta object code from reading C++ file 'cprogramdeviceview.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/widget/cprogramdeviceview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cprogramdeviceview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CProgramDeviceView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      37,   19,   19,   19, 0x08,
      61,   19,   19,   19, 0x08,
      76,   19,   19,   19, 0x08,
      94,   19,   19,   19, 0x08,
     110,   19,   19,   19, 0x08,
     122,   19,   19,   19, 0x08,
     135,   19,   19,   19, 0x08,
     152,   19,   19,   19, 0x08,
     168,   19,   19,   19, 0x08,
     190,   19,   19,   19, 0x08,
     211,   19,   19,   19, 0x08,
     234,   19,   19,   19, 0x08,
     257,   19,   19,   19, 0x08,
     279,   19,   19,   19, 0x08,
     306,  300,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CProgramDeviceView[] = {
    "CProgramDeviceView\0\0slot_ChangeAdd()\0"
    "slot_ChangeDeviceType()\0slot_Lighten()\0"
    "slot_CloseLight()\0slot_AllClose()\0"
    "slot_Left()\0slot_Right()\0slot_StartTest()\0"
    "slot_StopTest()\0slot_StartEmergency()\0"
    "slot_StopEmergency()\0slot_ReadLampVersion()\0"
    "slot_CloseBatteryOut()\0slot_BatteryOutKeep()\0"
    "slot_uploadOldLamp()\0index\0"
    "on_tableView_clicked(QModelIndex)\0"
};

void CProgramDeviceView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CProgramDeviceView *_t = static_cast<CProgramDeviceView *>(_o);
        switch (_id) {
        case 0: _t->slot_ChangeAdd(); break;
        case 1: _t->slot_ChangeDeviceType(); break;
        case 2: _t->slot_Lighten(); break;
        case 3: _t->slot_CloseLight(); break;
        case 4: _t->slot_AllClose(); break;
        case 5: _t->slot_Left(); break;
        case 6: _t->slot_Right(); break;
        case 7: _t->slot_StartTest(); break;
        case 8: _t->slot_StopTest(); break;
        case 9: _t->slot_StartEmergency(); break;
        case 10: _t->slot_StopEmergency(); break;
        case 11: _t->slot_ReadLampVersion(); break;
        case 12: _t->slot_CloseBatteryOut(); break;
        case 13: _t->slot_BatteryOutKeep(); break;
        case 14: _t->slot_uploadOldLamp(); break;
        case 15: _t->on_tableView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CProgramDeviceView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CProgramDeviceView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CProgramDeviceView,
      qt_meta_data_CProgramDeviceView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CProgramDeviceView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CProgramDeviceView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CProgramDeviceView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CProgramDeviceView))
        return static_cast<void*>(const_cast< CProgramDeviceView*>(this));
    return QWidget::qt_metacast(_clname);
}

int CProgramDeviceView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

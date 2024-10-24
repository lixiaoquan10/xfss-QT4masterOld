/****************************************************************************
** Meta object code from reading C++ file 'cprogramview.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/widget/cprogramview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cprogramview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CProgramView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      30,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   13,   13,   13, 0x0a,
      61,   13,   13,   13, 0x0a,
      76,   13,   13,   13, 0x0a,
      91,   13,   13,   13, 0x0a,
     110,   13,   13,   13, 0x0a,
     133,   13,   13,   13, 0x0a,
     157,   13,   13,   13, 0x0a,
     178,   13,   13,   13, 0x0a,
     200,   13,   13,   13, 0x0a,
     225,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CProgramView[] = {
    "CProgramView\0\0sigRegAllInfo()\0"
    "sigRegLoopInfo()\0slot_regALL()\0"
    "slot_regLoop()\0slot_unLogin()\0"
    "slot_unLoginLoop()\0slot_recodeAllDevice()\0"
    "slot_recodeLoopDevice()\0slot_saveAllDevice()\0"
    "slot_saveLoopDevice()\0slot_removeLoginObject()\0"
    "slot_resetControllerView()\0"
};

void CProgramView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CProgramView *_t = static_cast<CProgramView *>(_o);
        switch (_id) {
        case 0: _t->sigRegAllInfo(); break;
        case 1: _t->sigRegLoopInfo(); break;
        case 2: _t->slot_regALL(); break;
        case 3: _t->slot_regLoop(); break;
        case 4: _t->slot_unLogin(); break;
        case 5: _t->slot_unLoginLoop(); break;
        case 6: _t->slot_recodeAllDevice(); break;
        case 7: _t->slot_recodeLoopDevice(); break;
        case 8: _t->slot_saveAllDevice(); break;
        case 9: _t->slot_saveLoopDevice(); break;
        case 10: _t->slot_removeLoginObject(); break;
        case 11: _t->slot_resetControllerView(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CProgramView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CProgramView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CProgramView,
      qt_meta_data_CProgramView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CProgramView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CProgramView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CProgramView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CProgramView))
        return static_cast<void*>(const_cast< CProgramView*>(this));
    return QWidget::qt_metacast(_clname);
}

int CProgramView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void CProgramView::sigRegAllInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CProgramView::sigRegLoopInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE

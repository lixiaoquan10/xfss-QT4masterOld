/****************************************************************************
** Meta object code from reading C++ file 'cprogramtreeview.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/widget/cprogramtreeview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cprogramtreeview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CProgramTreeView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   17,   17,   17, 0x0a,
      75,   17,   17,   17, 0x0a,
      96,   17,   17,   17, 0x0a,
     118,   17,   17,   17, 0x0a,
     139,   17,   17,   17, 0x0a,
     182,  162,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CProgramTreeView[] = {
    "CProgramTreeView\0\0loop\0"
    "signalRightSelectedLoop(CLoop*)\0"
    "slotNewController()\0slotOpenController()\0"
    "slotCloseController()\0slotSaveController()\0"
    "slotExportController()\0selected,deselected\0"
    "slotSelectionChanged(QItemSelection,QItemSelection)\0"
};

void CProgramTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CProgramTreeView *_t = static_cast<CProgramTreeView *>(_o);
        switch (_id) {
        case 0: _t->signalRightSelectedLoop((*reinterpret_cast< CLoop*(*)>(_a[1]))); break;
        case 1: _t->slotNewController(); break;
        case 2: _t->slotOpenController(); break;
        case 3: _t->slotCloseController(); break;
        case 4: _t->slotSaveController(); break;
        case 5: _t->slotExportController(); break;
        case 6: _t->slotSelectionChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CProgramTreeView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CProgramTreeView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CProgramTreeView,
      qt_meta_data_CProgramTreeView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CProgramTreeView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CProgramTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CProgramTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CProgramTreeView))
        return static_cast<void*>(const_cast< CProgramTreeView*>(this));
    return QWidget::qt_metacast(_clname);
}

int CProgramTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CProgramTreeView::signalRightSelectedLoop(CLoop * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

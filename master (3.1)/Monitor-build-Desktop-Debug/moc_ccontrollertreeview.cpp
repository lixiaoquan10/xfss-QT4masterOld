/****************************************************************************
** Meta object code from reading C++ file 'ccontrollertreeview.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/view/ccontrollertreeview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccontrollertreeview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CControllerTreeView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      41,   21,   20,   20, 0x05,
     109,   92,   20,   20, 0x05,
     148,  142,   20,   20, 0x05,
     187,  142,   20,   20, 0x05,
     224,  142,   20,   20, 0x05,
     253,  142,   20,   20, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CControllerTreeView[] = {
    "CControllerTreeView\0\0selected,deselected\0"
    "sigSelectionChanged(QItemSelection,QItemSelection)\0"
    "supportedActions\0signalStartDrag(Qt::DropActions)\0"
    "event\0signalDragEnterEvent(QDragEnterEvent*)\0"
    "signalDragMoveEvent(QDragMoveEvent*)\0"
    "signalDropEvent(QDropEvent*)\0"
    "signalContextMenuEvent(QContextMenuEvent*)\0"
};

void CControllerTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CControllerTreeView *_t = static_cast<CControllerTreeView *>(_o);
        switch (_id) {
        case 0: _t->sigSelectionChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        case 1: _t->signalStartDrag((*reinterpret_cast< Qt::DropActions(*)>(_a[1]))); break;
        case 2: _t->signalDragEnterEvent((*reinterpret_cast< QDragEnterEvent*(*)>(_a[1]))); break;
        case 3: _t->signalDragMoveEvent((*reinterpret_cast< QDragMoveEvent*(*)>(_a[1]))); break;
        case 4: _t->signalDropEvent((*reinterpret_cast< QDropEvent*(*)>(_a[1]))); break;
        case 5: _t->signalContextMenuEvent((*reinterpret_cast< QContextMenuEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CControllerTreeView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CControllerTreeView::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_CControllerTreeView,
      qt_meta_data_CControllerTreeView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CControllerTreeView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CControllerTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CControllerTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CControllerTreeView))
        return static_cast<void*>(const_cast< CControllerTreeView*>(this));
    return QTreeView::qt_metacast(_clname);
}

int CControllerTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CControllerTreeView::sigSelectionChanged(const QItemSelection & _t1, const QItemSelection & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CControllerTreeView::signalStartDrag(Qt::DropActions _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CControllerTreeView::signalDragEnterEvent(QDragEnterEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CControllerTreeView::signalDragMoveEvent(QDragMoveEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CControllerTreeView::signalDropEvent(QDropEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CControllerTreeView::signalContextMenuEvent(QContextMenuEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'cdesigntreeview.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/widget/cdesigntreeview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cdesigntreeview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDesignTreeView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   17,   16,   16, 0x05,
      51,   45,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      73,   16,   16,   16, 0x0a,
      93,   16,   16,   16, 0x0a,
     114,   16,   16,   16, 0x0a,
     136,   16,   16,   16, 0x0a,
     157,   16,   16,   16, 0x0a,
     200,  180,   16,   16, 0x0a,
     269,  252,   16,   16, 0x0a,
     306,  300,   16,   16, 0x0a,
     343,  300,   16,   16, 0x0a,
     378,  300,   16,   16, 0x0a,
     405,   16,   16,   16, 0x08,
     416,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CDesignTreeView[] = {
    "CDesignTreeView\0\0enable\0sigIsLayerView(bool)\0"
    "layer\0layerChanged(CLayer*)\0"
    "slotNewController()\0slotOpenController()\0"
    "slotCloseController()\0slotSaveController()\0"
    "slotExportController()\0selected,deselected\0"
    "slotSelectionChanged(QItemSelection,QItemSelection)\0"
    "supportedActions\0slotStartDrag(Qt::DropActions)\0"
    "event\0slotDragEnterEvent(QDragEnterEvent*)\0"
    "slotDragMoveEvent(QDragMoveEvent*)\0"
    "slotDropEvent(QDropEvent*)\0addLayer()\0"
    "delLayer()\0"
};

void CDesignTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDesignTreeView *_t = static_cast<CDesignTreeView *>(_o);
        switch (_id) {
        case 0: _t->sigIsLayerView((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->layerChanged((*reinterpret_cast< CLayer*(*)>(_a[1]))); break;
        case 2: _t->slotNewController(); break;
        case 3: _t->slotOpenController(); break;
        case 4: _t->slotCloseController(); break;
        case 5: _t->slotSaveController(); break;
        case 6: _t->slotExportController(); break;
        case 7: _t->slotSelectionChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        case 8: _t->slotStartDrag((*reinterpret_cast< Qt::DropActions(*)>(_a[1]))); break;
        case 9: _t->slotDragEnterEvent((*reinterpret_cast< QDragEnterEvent*(*)>(_a[1]))); break;
        case 10: _t->slotDragMoveEvent((*reinterpret_cast< QDragMoveEvent*(*)>(_a[1]))); break;
        case 11: _t->slotDropEvent((*reinterpret_cast< QDropEvent*(*)>(_a[1]))); break;
        case 12: _t->addLayer(); break;
        case 13: _t->delLayer(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDesignTreeView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDesignTreeView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CDesignTreeView,
      qt_meta_data_CDesignTreeView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDesignTreeView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDesignTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDesignTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDesignTreeView))
        return static_cast<void*>(const_cast< CDesignTreeView*>(this));
    return QWidget::qt_metacast(_clname);
}

int CDesignTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void CDesignTreeView::sigIsLayerView(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDesignTreeView::layerChanged(CLayer * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

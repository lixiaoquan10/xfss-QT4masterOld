/****************************************************************************
** Meta object code from reading C++ file 'ccenterframe.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/widget/ccenterframe.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccenterframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCenterFrame[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x0a,
      51,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CCenterFrame[] = {
    "CCenterFrame\0\0index\0slotDoubleClicked(QModelIndex)\0"
    "slotToggleLeftBox()\0"
};

void CCenterFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCenterFrame *_t = static_cast<CCenterFrame *>(_o);
        switch (_id) {
        case 0: _t->slotDoubleClicked((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 1: _t->slotToggleLeftBox(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCenterFrame::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCenterFrame::staticMetaObject = {
    { &QSplitter::staticMetaObject, qt_meta_stringdata_CCenterFrame,
      qt_meta_data_CCenterFrame, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCenterFrame::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCenterFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCenterFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCenterFrame))
        return static_cast<void*>(const_cast< CCenterFrame*>(this));
    return QSplitter::qt_metacast(_clname);
}

int CCenterFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSplitter::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

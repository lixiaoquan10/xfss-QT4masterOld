/****************************************************************************
** Meta object code from reading C++ file 'ckeyboard.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/keyboard/ckeyboard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ckeyboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CKeyboard[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   10,   10,   10, 0x08,
      97,   79,   10,   10, 0x08,
     134,  132,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CKeyboard[] = {
    "CKeyboard\0\0character\0characterGenerated(quint32)\0"
    "on_pushButton_close_clicked()\0"
    "oldFocus,newFocus\0saveFocusWidget(QWidget*,QWidget*)\0"
    "w\0buttonClicked(QWidget*)\0"
};

void CKeyboard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CKeyboard *_t = static_cast<CKeyboard *>(_o);
        switch (_id) {
        case 0: _t->characterGenerated((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 1: _t->on_pushButton_close_clicked(); break;
        case 2: _t->saveFocusWidget((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 3: _t->buttonClicked((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CKeyboard::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CKeyboard::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CKeyboard,
      qt_meta_data_CKeyboard, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CKeyboard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CKeyboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CKeyboard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CKeyboard))
        return static_cast<void*>(const_cast< CKeyboard*>(this));
    return QDialog::qt_metacast(_clname);
}

int CKeyboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CKeyboard::characterGenerated(quint32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

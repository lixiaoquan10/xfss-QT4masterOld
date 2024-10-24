/****************************************************************************
** Meta object code from reading C++ file 'dlgsystemrecord.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/dialog/dlgsystemrecord.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgsystemrecord.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dlgSystemRecord[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      41,   16,   16,   16, 0x08,
      72,   16,   16,   16, 0x08,
     101,   16,   16,   16, 0x08,
     138,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_dlgSystemRecord[] = {
    "dlgSystemRecord\0\0on_pushButton_clicked()\0"
    "on_pushButton_before_clicked()\0"
    "on_pushButton_next_clicked()\0"
    "on_pushButton_before_query_clicked()\0"
    "on_pushButton_next_query_clicked()\0"
};

void dlgSystemRecord::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dlgSystemRecord *_t = static_cast<dlgSystemRecord *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_pushButton_before_clicked(); break;
        case 2: _t->on_pushButton_next_clicked(); break;
        case 3: _t->on_pushButton_before_query_clicked(); break;
        case 4: _t->on_pushButton_next_query_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData dlgSystemRecord::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dlgSystemRecord::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_dlgSystemRecord,
      qt_meta_data_dlgSystemRecord, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dlgSystemRecord::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dlgSystemRecord::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dlgSystemRecord::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dlgSystemRecord))
        return static_cast<void*>(const_cast< dlgSystemRecord*>(this));
    return QDialog::qt_metacast(_clname);
}

int dlgSystemRecord::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

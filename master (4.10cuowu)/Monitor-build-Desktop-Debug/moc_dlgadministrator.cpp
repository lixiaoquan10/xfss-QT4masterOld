/****************************************************************************
** Meta object code from reading C++ file 'dlgadministrator.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/dialog/dlgadministrator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgadministrator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dlgAdministrator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      44,   17,   17,   17, 0x08,
      70,   17,   17,   17, 0x08,
      96,   17,   17,   17, 0x08,
     122,   17,   17,   17, 0x08,
     148,   17,   17,   17, 0x08,
     174,   17,   17,   17, 0x08,
     200,   17,   17,   17, 0x08,
     226,   17,   17,   17, 0x08,
     252,   17,   17,   17, 0x08,
     283,   17,   17,   17, 0x08,
     309,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_dlgAdministrator[] = {
    "dlgAdministrator\0\0on_pushButton_1_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_3_clicked()\0"
    "on_pushButton_4_clicked()\0"
    "on_pushButton_5_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_pushButton_7_clicked()\0"
    "on_pushButton_8_clicked()\0"
    "on_pushButton_9_clicked()\0"
    "on_pushButton_cancel_clicked()\0"
    "on_pushButton_0_clicked()\0"
    "on_pushButton_empty_clicked()\0"
};

void dlgAdministrator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dlgAdministrator *_t = static_cast<dlgAdministrator *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_1_clicked(); break;
        case 1: _t->on_pushButton_2_clicked(); break;
        case 2: _t->on_pushButton_3_clicked(); break;
        case 3: _t->on_pushButton_4_clicked(); break;
        case 4: _t->on_pushButton_5_clicked(); break;
        case 5: _t->on_pushButton_6_clicked(); break;
        case 6: _t->on_pushButton_7_clicked(); break;
        case 7: _t->on_pushButton_8_clicked(); break;
        case 8: _t->on_pushButton_9_clicked(); break;
        case 9: _t->on_pushButton_cancel_clicked(); break;
        case 10: _t->on_pushButton_0_clicked(); break;
        case 11: _t->on_pushButton_empty_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData dlgAdministrator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dlgAdministrator::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_dlgAdministrator,
      qt_meta_data_dlgAdministrator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dlgAdministrator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dlgAdministrator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dlgAdministrator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dlgAdministrator))
        return static_cast<void*>(const_cast< dlgAdministrator*>(this));
    return QDialog::qt_metacast(_clname);
}

int dlgAdministrator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'dlghandfireview.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/dialog/dlghandfireview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlghandfireview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dlgHandFireView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      47,   16,   16,   16, 0x08,
      76,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_dlgHandFireView[] = {
    "dlgHandFireView\0\0on_pushButton_close_clicked()\0"
    "on_pushButton_fire_clicked()\0"
    "slot_threadFinished()\0"
};

void dlgHandFireView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dlgHandFireView *_t = static_cast<dlgHandFireView *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_close_clicked(); break;
        case 1: _t->on_pushButton_fire_clicked(); break;
        case 2: _t->slot_threadFinished(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData dlgHandFireView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dlgHandFireView::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_dlgHandFireView,
      qt_meta_data_dlgHandFireView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dlgHandFireView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dlgHandFireView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dlgHandFireView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dlgHandFireView))
        return static_cast<void*>(const_cast< dlgHandFireView*>(this));
    return QDialog::qt_metacast(_clname);
}

int dlgHandFireView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_LoadNodeDataThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_LoadNodeDataThread[] = {
    "LoadNodeDataThread\0\0dataLoaded()\0"
};

void LoadNodeDataThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LoadNodeDataThread *_t = static_cast<LoadNodeDataThread *>(_o);
        switch (_id) {
        case 0: _t->dataLoaded(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData LoadNodeDataThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LoadNodeDataThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_LoadNodeDataThread,
      qt_meta_data_LoadNodeDataThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LoadNodeDataThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LoadNodeDataThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LoadNodeDataThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LoadNodeDataThread))
        return static_cast<void*>(const_cast< LoadNodeDataThread*>(this));
    return QThread::qt_metacast(_clname);
}

int LoadNodeDataThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void LoadNodeDataThread::dataLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'cprocesssocket.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BService/process/cprocesssocket.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cprocesssocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CprocessSocket[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      33,   15,   15,   15, 0x0a,
      50,   15,   15,   15, 0x0a,
      67,   15,   15,   15, 0x0a,
      82,   15,   15,   15, 0x0a,
     106,  100,   15,   15, 0x0a,
     154,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CprocessSocket[] = {
    "CprocessSocket\0\0slot_reconnect()\0"
    "slot_heartBeat()\0slot_readyRead()\0"
    "slot_connect()\0slot_disconnect()\0state\0"
    "slot_stateChanged(QAbstractSocket::SocketState)\0"
    "slot_controlCloudTxtFileSize()\0"
};

void CprocessSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CprocessSocket *_t = static_cast<CprocessSocket *>(_o);
        switch (_id) {
        case 0: _t->slot_reconnect(); break;
        case 1: _t->slot_heartBeat(); break;
        case 2: _t->slot_readyRead(); break;
        case 3: _t->slot_connect(); break;
        case 4: _t->slot_disconnect(); break;
        case 5: _t->slot_stateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 6: _t->slot_controlCloudTxtFileSize(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CprocessSocket::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CprocessSocket::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CprocessSocket,
      qt_meta_data_CprocessSocket, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CprocessSocket::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CprocessSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CprocessSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CprocessSocket))
        return static_cast<void*>(const_cast< CprocessSocket*>(this));
    return QObject::qt_metacast(_clname);
}

int CprocessSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'cprocessserver.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BService/process/cprocessserver.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cprocessserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CprocessServer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      38,   15,   15,   15, 0x0a,
      65,   15,   15,   15, 0x0a,
      85,   15,   15,   15, 0x0a,
     117,  111,   15,   15, 0x0a,
     161,   15,   15,   15, 0x0a,
     184,  178,   15,   15, 0x0a,
     211,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CprocessServer[] = {
    "CprocessServer\0\0slot_readClientData()\0"
    "slot_handleNewConnection()\0"
    "slot_serverlisten()\0slot_handleDisconnected()\0"
    "error\0onSocketError(QAbstractSocket::SocketError)\0"
    "slot_heartBeat()\0bytes\0"
    "handleBytesWritten(qint64)\0"
    "slot_controlMasterTxtFileSize()\0"
};

void CprocessServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CprocessServer *_t = static_cast<CprocessServer *>(_o);
        switch (_id) {
        case 0: _t->slot_readClientData(); break;
        case 1: _t->slot_handleNewConnection(); break;
        case 2: _t->slot_serverlisten(); break;
        case 3: _t->slot_handleDisconnected(); break;
        case 4: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: _t->slot_heartBeat(); break;
        case 6: _t->handleBytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 7: _t->slot_controlMasterTxtFileSize(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CprocessServer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CprocessServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CprocessServer,
      qt_meta_data_CprocessServer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CprocessServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CprocessServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CprocessServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CprocessServer))
        return static_cast<void*>(const_cast< CprocessServer*>(this));
    return QObject::qt_metacast(_clname);
}

int CprocessServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'ccommunicationroute.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BService/process/ccommunicationroute.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccommunicationroute.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCommunicationRoute[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      58,   21,   20,   20, 0x05,
     122,   21,   20,   20, 0x05,
     214,  184,   20,   20, 0x05,
     323,  289,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
     422,  388,   20,   20, 0x0a,
     528,  491,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CCommunicationRoute[] = {
    "CCommunicationRoute\0\0"
    "lineName,infotype,controlDomain,data\0"
    "sendCommandData(QString,int,QHash<QString,QVariant>,QByteArray)\0"
    "sendPrintData(QString,int,QHash<QString,QVariant>,QByteArray)\0"
    "name,commandName,control,data\0"
    "communicationSendEvent(QString,QString,QHash<QString,QVariant>,QByteAr"
    "ray)\0"
    "processname,infotype,control,data\0"
    "processRecvEvent(QString,int,QHash<QString,QVariant>,QByteArray)\0"
    "processname,infoType,control,data\0"
    "procProcessSendEvent(QString,int,QHash<QString,QVariant>,QByteArray)\0"
    "lineName,infoType,controlDomain,data\0"
    "procCommunicationRecvEvent(QString,int,QHash<QString,QVariant>,QByteAr"
    "ray)\0"
};

void CCommunicationRoute::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCommunicationRoute *_t = static_cast<CCommunicationRoute *>(_o);
        switch (_id) {
        case 0: _t->sendCommandData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QHash<QString,QVariant>(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 1: _t->sendPrintData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QHash<QString,QVariant>(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 2: _t->communicationSendEvent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QHash<QString,QVariant>(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 3: _t->processRecvEvent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QHash<QString,QVariant>(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 4: _t->procProcessSendEvent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QHash<QString,QVariant>(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 5: _t->procCommunicationRecvEvent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QHash<QString,QVariant>(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCommunicationRoute::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCommunicationRoute::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CCommunicationRoute,
      qt_meta_data_CCommunicationRoute, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCommunicationRoute::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCommunicationRoute::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCommunicationRoute::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCommunicationRoute))
        return static_cast<void*>(const_cast< CCommunicationRoute*>(this));
    return QObject::qt_metacast(_clname);
}

int CCommunicationRoute::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void CCommunicationRoute::sendCommandData(const QString & _t1, const int & _t2, const QHash<QString,QVariant> & _t3, const QByteArray & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CCommunicationRoute::sendPrintData(const QString & _t1, const int & _t2, const QHash<QString,QVariant> & _t3, const QByteArray & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CCommunicationRoute::communicationSendEvent(const QString & _t1, const QString & _t2, const QHash<QString,QVariant> & _t3, const QByteArray & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CCommunicationRoute::processRecvEvent(const QString & _t1, const int & _t2, const QHash<QString,QVariant> & _t3, const QByteArray & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE

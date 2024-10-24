/****************************************************************************
** Meta object code from reading C++ file 'tablewidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/dialog/tablewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tablewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TableWidget[] = {

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
      36,   18,   13,   12, 0x0a,
      88,   76,   13,   12, 0x2a,
     135,  121,   12,   12, 0x0a,
     179,  171,   12,   12, 0x2a,
     212,  208,   12,   12, 0x0a,
     233,   12,   12,   12, 0x0a,
     276,  255,   12,   12, 0x0a,
     316,  305,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TableWidget[] = {
    "TableWidget\0\0bool\0row,content,brush\0"
    "InsertSingleRow(int,QStringList,QBrush)\0"
    "row,content\0InsertSingleRow(int,QStringList)\0"
    "content,brush\0AppendSingleRow(QStringList,QBrush)\0"
    "content\0AppendSingleRow(QStringList)\0"
    "row\0RemoveSingleRow(int)\0RemoveLastSingleRow()\0"
    "nrow,ncolumn,strtext\0SetItemText(int,int,QString)\0"
    "nrow,color\0SetRowColor(int,QColor)\0"
};

void TableWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TableWidget *_t = static_cast<TableWidget *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->InsertSingleRow((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< QBrush(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = _t->InsertSingleRow((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: _t->AppendSingleRow((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QBrush(*)>(_a[2]))); break;
        case 3: _t->AppendSingleRow((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 4: _t->RemoveSingleRow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->RemoveLastSingleRow(); break;
        case 6: _t->SetItemText((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->SetRowColor((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QColor(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TableWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TableWidget::staticMetaObject = {
    { &QTableWidget::staticMetaObject, qt_meta_stringdata_TableWidget,
      qt_meta_data_TableWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TableWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TableWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TableWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TableWidget))
        return static_cast<void*>(const_cast< TableWidget*>(this));
    return QTableWidget::qt_metacast(_clname);
}

int TableWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
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

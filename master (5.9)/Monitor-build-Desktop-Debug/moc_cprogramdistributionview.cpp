/****************************************************************************
** Meta object code from reading C++ file 'cprogramdistributionview.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Monitor/widget/cprogramdistributionview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cprogramdistributionview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CProgramDistributionView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   25,   25,   25, 0x08,
      44,   25,   25,   25, 0x08,
      65,   59,   25,   25, 0x08,
     107,   59,   25,   25, 0x08,
     146,   59,   25,   25, 0x08,
     195,  187,   25,   25, 0x08,
     230,  187,   25,   25, 0x08,
     269,  263,   25,   25, 0x08,
     316,  263,   25,   25, 0x08,
     366,  263,   25,   25, 0x08,
     417,   25,   25,   25, 0x08,
     451,   25,   25,   25, 0x08,
     484,   25,   25,   25, 0x08,
     522,   25,   25,   25, 0x08,
     557,   25,   25,   25, 0x08,
     591,   25,   25,   25, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CProgramDistributionView[] = {
    "CProgramDistributionView\0\0readProtectInfo()\0"
    "updateStatus()\0value\0"
    "on_m_opencurrent_slider_valueChanged(int)\0"
    "on_m_firetime_slider_valueChanged(int)\0"
    "on_m_unfiretime_slider_valueChanged(int)\0"
    "checked\0on_CAN_communication_clicked(bool)\0"
    "on_light_detection_clicked(bool)\0index\0"
    "on_comboBox_powermode_currentIndexChanged(int)\0"
    "on_comboBox_linkageInput_currentIndexChanged(int)\0"
    "on_comboBox_loopoutstatus_currentIndexChanged(int)\0"
    "on_closepowerout_button_clicked()\0"
    "on_poweroutkeep_button_clicked()\0"
    "on_readloopoutstatus_button_clicked()\0"
    "on_emergencyStart_button_clicked()\0"
    "on_emergencyStop_button_clicked()\0"
    "on_removeLampLogin_button_clicked()\0"
};

void CProgramDistributionView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CProgramDistributionView *_t = static_cast<CProgramDistributionView *>(_o);
        switch (_id) {
        case 0: _t->readProtectInfo(); break;
        case 1: _t->updateStatus(); break;
        case 2: _t->on_m_opencurrent_slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_m_firetime_slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_m_unfiretime_slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_CAN_communication_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_light_detection_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_comboBox_powermode_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_comboBox_linkageInput_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_comboBox_loopoutstatus_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_closepowerout_button_clicked(); break;
        case 11: _t->on_poweroutkeep_button_clicked(); break;
        case 12: _t->on_readloopoutstatus_button_clicked(); break;
        case 13: _t->on_emergencyStart_button_clicked(); break;
        case 14: _t->on_emergencyStop_button_clicked(); break;
        case 15: _t->on_removeLampLogin_button_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CProgramDistributionView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CProgramDistributionView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CProgramDistributionView,
      qt_meta_data_CProgramDistributionView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CProgramDistributionView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CProgramDistributionView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CProgramDistributionView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CProgramDistributionView))
        return static_cast<void*>(const_cast< CProgramDistributionView*>(this));
    return QWidget::qt_metacast(_clname);
}

int CProgramDistributionView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'FactoryTime.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/System/FactoryTime.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FactoryTime.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__FactoryTimeWidget_t {
    QByteArrayData data[5];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__FactoryTimeWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__FactoryTimeWidget_t qt_meta_stringdata_UI__FactoryTimeWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "UI::FactoryTimeWidget"
QT_MOC_LITERAL(1, 22, 11), // "ViewRefresh"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 18), // "SlotDefaultFactory"
QT_MOC_LITERAL(4, 54, 19) // "SlotScreenCalibrate"

    },
    "UI::FactoryTimeWidget\0ViewRefresh\0\0"
    "SlotDefaultFactory\0SlotScreenCalibrate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__FactoryTimeWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UI::FactoryTimeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FactoryTimeWidget *_t = static_cast<FactoryTimeWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ViewRefresh(); break;
        case 1: _t->SlotDefaultFactory(); break;
        case 2: _t->SlotScreenCalibrate(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FactoryTimeWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FactoryTimeWidget::ViewRefresh)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject UI::FactoryTimeWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UI__FactoryTimeWidget.data,
      qt_meta_data_UI__FactoryTimeWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::FactoryTimeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::FactoryTimeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__FactoryTimeWidget.stringdata0))
        return static_cast<void*>(const_cast< FactoryTimeWidget*>(this));
    if (!strcmp(_clname, "IUserChangeNotifiable"))
        return static_cast< IUserChangeNotifiable*>(const_cast< FactoryTimeWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int UI::FactoryTimeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void UI::FactoryTimeWidget::ViewRefresh()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

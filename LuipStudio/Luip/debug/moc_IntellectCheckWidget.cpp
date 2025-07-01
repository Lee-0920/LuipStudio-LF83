/****************************************************************************
** Meta object code from reading C++ file 'IntellectCheckWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/Maintain/IntellectCheckWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IntellectCheckWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__IntellectCheck_t {
    QByteArrayData data[5];
    char stringdata0[68];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__IntellectCheck_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__IntellectCheck_t qt_meta_stringdata_UI__IntellectCheck = {
    {
QT_MOC_LITERAL(0, 0, 18), // "UI::IntellectCheck"
QT_MOC_LITERAL(1, 19, 14), // "OneCheckConfig"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 14), // "SlotStopDetect"
QT_MOC_LITERAL(4, 50, 17) // "OneCheckAllConfig"

    },
    "UI::IntellectCheck\0OneCheckConfig\0\0"
    "SlotStopDetect\0OneCheckAllConfig"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__IntellectCheck[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UI::IntellectCheck::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IntellectCheck *_t = static_cast<IntellectCheck *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OneCheckConfig(); break;
        case 1: _t->SlotStopDetect(); break;
        case 2: _t->OneCheckAllConfig(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject UI::IntellectCheck::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UI__IntellectCheck.data,
      qt_meta_data_UI__IntellectCheck,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::IntellectCheck::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::IntellectCheck::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__IntellectCheck.stringdata0))
        return static_cast<void*>(const_cast< IntellectCheck*>(this));
    if (!strcmp(_clname, "IUserChangeNotifiable"))
        return static_cast< IUserChangeNotifiable*>(const_cast< IntellectCheck*>(this));
    if (!strcmp(_clname, "IFlowNotifiable"))
        return static_cast< IFlowNotifiable*>(const_cast< IntellectCheck*>(this));
    return QWidget::qt_metacast(_clname);
}

int UI::IntellectCheck::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE

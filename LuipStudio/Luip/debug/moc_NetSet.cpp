/****************************************************************************
** Meta object code from reading C++ file 'NetSet.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/System/NetSet.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetSet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__NetSet_t {
    QByteArrayData data[8];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__NetSet_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__NetSet_t qt_meta_stringdata_UI__NetSet = {
    {
QT_MOC_LITERAL(0, 0, 10), // "UI::NetSet"
QT_MOC_LITERAL(1, 11, 19), // "UpdateCurrentIPInfo"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 8), // "NetApply"
QT_MOC_LITERAL(4, 41, 12), // "OnRadioClick"
QT_MOC_LITERAL(5, 54, 12), // "TimerHandler"
QT_MOC_LITERAL(6, 67, 10), // "DHCPFinish"
QT_MOC_LITERAL(7, 78, 8) // "exitCode"

    },
    "UI::NetSet\0UpdateCurrentIPInfo\0\0"
    "NetApply\0OnRadioClick\0TimerHandler\0"
    "DHCPFinish\0exitCode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__NetSet[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    1,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

void UI::NetSet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetSet *_t = static_cast<NetSet *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdateCurrentIPInfo(); break;
        case 1: _t->NetApply(); break;
        case 2: _t->OnRadioClick(); break;
        case 3: _t->TimerHandler(); break;
        case 4: _t->DHCPFinish((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject UI::NetSet::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UI__NetSet.data,
      qt_meta_data_UI__NetSet,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::NetSet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::NetSet::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__NetSet.stringdata0))
        return static_cast<void*>(const_cast< NetSet*>(this));
    if (!strcmp(_clname, "IUserChangeNotifiable"))
        return static_cast< IUserChangeNotifiable*>(const_cast< NetSet*>(this));
    return QWidget::qt_metacast(_clname);
}

int UI::NetSet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

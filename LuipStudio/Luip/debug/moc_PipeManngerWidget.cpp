/****************************************************************************
** Meta object code from reading C++ file 'PipeManngerWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/Maintain/PipeManngerWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PipeManngerWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__PipeManngerWidget_t {
    QByteArrayData data[10];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__PipeManngerWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__PipeManngerWidget_t qt_meta_stringdata_UI__PipeManngerWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "UI::PipeManngerWidget"
QT_MOC_LITERAL(1, 22, 12), // "MeasureStart"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 5), // "index"
QT_MOC_LITERAL(4, 42, 8), // "ChoseDay"
QT_MOC_LITERAL(5, 51, 11), // "StopMeasure"
QT_MOC_LITERAL(6, 63, 10), // "CheckValue"
QT_MOC_LITERAL(7, 74, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(8, 92, 4), // "item"
QT_MOC_LITERAL(9, 97, 16) // "SlotIndexChanged"

    },
    "UI::PipeManngerWidget\0MeasureStart\0\0"
    "index\0ChoseDay\0StopMeasure\0CheckValue\0"
    "QTableWidgetItem*\0item\0SlotIndexChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__PipeManngerWidget[] = {

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
       1,    1,   39,    2, 0x08 /* Private */,
       4,    0,   42,    2, 0x08 /* Private */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    1,   44,    2, 0x08 /* Private */,
       9,    1,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void UI::PipeManngerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PipeManngerWidget *_t = static_cast<PipeManngerWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->MeasureStart((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->ChoseDay(); break;
        case 2: _t->StopMeasure(); break;
        case 3: _t->CheckValue((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->SlotIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject UI::PipeManngerWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UI__PipeManngerWidget.data,
      qt_meta_data_UI__PipeManngerWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::PipeManngerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::PipeManngerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__PipeManngerWidget.stringdata0))
        return static_cast<void*>(const_cast< PipeManngerWidget*>(this));
    if (!strcmp(_clname, "IUserChangeNotifiable"))
        return static_cast< IUserChangeNotifiable*>(const_cast< PipeManngerWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int UI::PipeManngerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

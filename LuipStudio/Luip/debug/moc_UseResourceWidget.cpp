/****************************************************************************
** Meta object code from reading C++ file 'UseResourceWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/Maintain/UseResourceWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UseResourceWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__UseResourceWidget_t {
    QByteArrayData data[8];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__UseResourceWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__UseResourceWidget_t qt_meta_stringdata_UI__UseResourceWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "UI::UseResourceWidget"
QT_MOC_LITERAL(1, 22, 10), // "UseMeasure"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 17), // "slotDoubleClicked"
QT_MOC_LITERAL(4, 52, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(5, 70, 4), // "item"
QT_MOC_LITERAL(6, 75, 10), // "CheckValue"
QT_MOC_LITERAL(7, 86, 21) // "SlotMaterialLifeCheck"

    },
    "UI::UseResourceWidget\0UseMeasure\0\0"
    "slotDoubleClicked\0QTableWidgetItem*\0"
    "item\0CheckValue\0SlotMaterialLifeCheck"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__UseResourceWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       3,    1,   37,    2, 0x08 /* Private */,
       6,    1,   40,    2, 0x08 /* Private */,
       7,    0,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,

       0        // eod
};

void UI::UseResourceWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UseResourceWidget *_t = static_cast<UseResourceWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UseMeasure((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slotDoubleClicked((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->CheckValue((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->SlotMaterialLifeCheck(); break;
        default: ;
        }
    }
}

const QMetaObject UI::UseResourceWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UI__UseResourceWidget.data,
      qt_meta_data_UI__UseResourceWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::UseResourceWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::UseResourceWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__UseResourceWidget.stringdata0))
        return static_cast<void*>(const_cast< UseResourceWidget*>(this));
    if (!strcmp(_clname, "IUserChangeNotifiable"))
        return static_cast< IUserChangeNotifiable*>(const_cast< UseResourceWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int UI::UseResourceWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

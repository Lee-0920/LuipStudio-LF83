/****************************************************************************
** Meta object code from reading C++ file 'AutoMeasureWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/Setting/AutoMeasureWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AutoMeasureWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__AutoMeasureWidget_t {
    QByteArrayData data[9];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__AutoMeasureWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__AutoMeasureWidget_t qt_meta_stringdata_UI__AutoMeasureWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "UI::AutoMeasureWidget"
QT_MOC_LITERAL(1, 22, 18), // "DefaultRestoreSlot"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 17), // "ConfigRefreshSlot"
QT_MOC_LITERAL(4, 60, 17), // "DoubleClickedSlot"
QT_MOC_LITERAL(5, 78, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(6, 96, 4), // "item"
QT_MOC_LITERAL(7, 101, 14), // "CheckValueSlot"
QT_MOC_LITERAL(8, 116, 15) // "OneCheckAllSlot"

    },
    "UI::AutoMeasureWidget\0DefaultRestoreSlot\0"
    "\0ConfigRefreshSlot\0DoubleClickedSlot\0"
    "QTableWidgetItem*\0item\0CheckValueSlot\0"
    "OneCheckAllSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__AutoMeasureWidget[] = {

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
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    1,   41,    2, 0x08 /* Private */,
       7,    1,   44,    2, 0x08 /* Private */,
       8,    0,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,

       0        // eod
};

void UI::AutoMeasureWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AutoMeasureWidget *_t = static_cast<AutoMeasureWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->DefaultRestoreSlot(); break;
        case 1: _t->ConfigRefreshSlot(); break;
        case 2: _t->DoubleClickedSlot((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->CheckValueSlot((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->OneCheckAllSlot(); break;
        default: ;
        }
    }
}

const QMetaObject UI::AutoMeasureWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UI__AutoMeasureWidget.data,
      qt_meta_data_UI__AutoMeasureWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::AutoMeasureWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::AutoMeasureWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__AutoMeasureWidget.stringdata0))
        return static_cast<void*>(const_cast< AutoMeasureWidget*>(this));
    if (!strcmp(_clname, "IUserChangeNotifiable"))
        return static_cast< IUserChangeNotifiable*>(const_cast< AutoMeasureWidget*>(this));
    if (!strcmp(_clname, "IUpdateWidgetNotifiable"))
        return static_cast< IUpdateWidgetNotifiable*>(const_cast< AutoMeasureWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int UI::AutoMeasureWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

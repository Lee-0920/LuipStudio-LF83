/****************************************************************************
** Meta object code from reading C++ file 'ParameterWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/Setting/ParameterWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ParameterWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__ParameterWidget_t {
    QByteArrayData data[8];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__ParameterWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__ParameterWidget_t qt_meta_stringdata_UI__ParameterWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "UI::ParameterWidget"
QT_MOC_LITERAL(1, 20, 18), // "DefaultRestoreSlot"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 17), // "ConfigRefreshSlot"
QT_MOC_LITERAL(4, 58, 17), // "DoubleClickedSlot"
QT_MOC_LITERAL(5, 76, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(6, 94, 4), // "item"
QT_MOC_LITERAL(7, 99, 14) // "CheckValueSlot"

    },
    "UI::ParameterWidget\0DefaultRestoreSlot\0"
    "\0ConfigRefreshSlot\0DoubleClickedSlot\0"
    "QTableWidgetItem*\0item\0CheckValueSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__ParameterWidget[] = {

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
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    1,   36,    2, 0x08 /* Private */,
       7,    1,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void UI::ParameterWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ParameterWidget *_t = static_cast<ParameterWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->DefaultRestoreSlot(); break;
        case 1: _t->ConfigRefreshSlot(); break;
        case 2: _t->DoubleClickedSlot((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->CheckValueSlot((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject UI::ParameterWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UI__ParameterWidget.data,
      qt_meta_data_UI__ParameterWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::ParameterWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::ParameterWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__ParameterWidget.stringdata0))
        return static_cast<void*>(const_cast< ParameterWidget*>(this));
    if (!strcmp(_clname, "IUserChangeNotifiable"))
        return static_cast< IUserChangeNotifiable*>(const_cast< ParameterWidget*>(this));
    if (!strcmp(_clname, "IUpdateWidgetNotifiable"))
        return static_cast< IUpdateWidgetNotifiable*>(const_cast< ParameterWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int UI::ParameterWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

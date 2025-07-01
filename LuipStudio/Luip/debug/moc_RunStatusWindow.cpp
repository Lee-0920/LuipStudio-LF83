/****************************************************************************
** Meta object code from reading C++ file 'RunStatusWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/Home/RunStatusWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RunStatusWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RunStatusWindow_t {
    QByteArrayData data[10];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RunStatusWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RunStatusWindow_t qt_meta_stringdata_RunStatusWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RunStatusWindow"
QT_MOC_LITERAL(1, 16, 14), // "ChangeReagent1"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 1), // "i"
QT_MOC_LITERAL(4, 34, 26), // "SignalOnMeasureResultAdded"
QT_MOC_LITERAL(5, 61, 8), // "StopFlow"
QT_MOC_LITERAL(6, 70, 16), // "SlotOnlineButton"
QT_MOC_LITERAL(7, 87, 16), // "SlotOneKeyButton"
QT_MOC_LITERAL(8, 104, 11), // "RefreshView"
QT_MOC_LITERAL(9, 116, 24) // "SlotOnMeasureResultAdded"

    },
    "RunStatusWindow\0ChangeReagent1\0\0i\0"
    "SignalOnMeasureResultAdded\0StopFlow\0"
    "SlotOnlineButton\0SlotOneKeyButton\0"
    "RefreshView\0SlotOnMeasureResultAdded"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RunStatusWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    0,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   53,    2, 0x08 /* Private */,
       6,    0,   54,    2, 0x08 /* Private */,
       7,    0,   55,    2, 0x08 /* Private */,
       8,    0,   56,    2, 0x08 /* Private */,
       9,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RunStatusWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RunStatusWindow *_t = static_cast<RunStatusWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ChangeReagent1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->SignalOnMeasureResultAdded(); break;
        case 2: _t->StopFlow(); break;
        case 3: _t->SlotOnlineButton(); break;
        case 4: _t->SlotOneKeyButton(); break;
        case 5: _t->RefreshView(); break;
        case 6: _t->SlotOnMeasureResultAdded(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RunStatusWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunStatusWindow::ChangeReagent1)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RunStatusWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunStatusWindow::SignalOnMeasureResultAdded)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject RunStatusWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RunStatusWindow.data,
      qt_meta_data_RunStatusWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RunStatusWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RunStatusWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RunStatusWindow.stringdata0))
        return static_cast<void*>(const_cast< RunStatusWindow*>(this));
    if (!strcmp(_clname, "IScheduleNotifiable"))
        return static_cast< IScheduleNotifiable*>(const_cast< RunStatusWindow*>(this));
    if (!strcmp(_clname, "Result::IResultNotifiable"))
        return static_cast< Result::IResultNotifiable*>(const_cast< RunStatusWindow*>(this));
    if (!strcmp(_clname, "IUserChangeNotifiable"))
        return static_cast< IUserChangeNotifiable*>(const_cast< RunStatusWindow*>(this));
    if (!strcmp(_clname, "IStatusNotifiable"))
        return static_cast< IStatusNotifiable*>(const_cast< RunStatusWindow*>(this));
    if (!strcmp(_clname, "IUpdateWidgetNotifiable"))
        return static_cast< IUpdateWidgetNotifiable*>(const_cast< RunStatusWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int RunStatusWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void RunStatusWindow::ChangeReagent1(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RunStatusWindow::SignalOnMeasureResultAdded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

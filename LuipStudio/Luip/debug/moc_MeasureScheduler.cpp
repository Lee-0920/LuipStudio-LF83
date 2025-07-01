/****************************************************************************
** Meta object code from reading C++ file 'MeasureScheduler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FlowManager/MeasureScheduler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MeasureScheduler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Flow__MeasureScheduler_t {
    QByteArrayData data[5];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Flow__MeasureScheduler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Flow__MeasureScheduler_t qt_meta_stringdata_Flow__MeasureScheduler = {
    {
QT_MOC_LITERAL(0, 0, 22), // "Flow::MeasureScheduler"
QT_MOC_LITERAL(1, 23, 12), // "SignalUpdate"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 10), // "SlotUpdate"
QT_MOC_LITERAL(4, 48, 11) // "SlotTimeout"

    },
    "Flow::MeasureScheduler\0SignalUpdate\0"
    "\0SlotUpdate\0SlotTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Flow__MeasureScheduler[] = {

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
       3,    0,   30,    2, 0x0a /* Public */,
       4,    0,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Flow::MeasureScheduler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MeasureScheduler *_t = static_cast<MeasureScheduler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SignalUpdate(); break;
        case 1: _t->SlotUpdate(); break;
        case 2: _t->SlotTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MeasureScheduler::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MeasureScheduler::SignalUpdate)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Flow::MeasureScheduler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Flow__MeasureScheduler.data,
      qt_meta_data_Flow__MeasureScheduler,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Flow::MeasureScheduler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Flow::MeasureScheduler::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Flow__MeasureScheduler.stringdata0))
        return static_cast<void*>(const_cast< MeasureScheduler*>(this));
    if (!strcmp(_clname, "IFlowNotifiable"))
        return static_cast< IFlowNotifiable*>(const_cast< MeasureScheduler*>(this));
    return QObject::qt_metacast(_clname);
}

int Flow::MeasureScheduler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void Flow::MeasureScheduler::SignalUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'UpdateWidgetManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/Frame/UpdateWidgetManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UpdateWidgetManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__UpdateWidgetManager_t {
    QByteArrayData data[6];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__UpdateWidgetManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__UpdateWidgetManager_t qt_meta_stringdata_UI__UpdateWidgetManager = {
    {
QT_MOC_LITERAL(0, 0, 23), // "UI::UpdateWidgetManager"
QT_MOC_LITERAL(1, 24, 12), // "UpdateWidget"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 5), // "event"
QT_MOC_LITERAL(4, 44, 7), // "message"
QT_MOC_LITERAL(5, 52, 16) // "SlotUpdateWidget"

    },
    "UI::UpdateWidgetManager\0UpdateWidget\0"
    "\0event\0message\0SlotUpdateWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__UpdateWidgetManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   29,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,

       0        // eod
};

void UI::UpdateWidgetManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UpdateWidgetManager *_t = static_cast<UpdateWidgetManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdateWidget((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->SlotUpdateWidget((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (UpdateWidgetManager::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UpdateWidgetManager::UpdateWidget)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject UI::UpdateWidgetManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UI__UpdateWidgetManager.data,
      qt_meta_data_UI__UpdateWidgetManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::UpdateWidgetManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::UpdateWidgetManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__UpdateWidgetManager.stringdata0))
        return static_cast<void*>(const_cast< UpdateWidgetManager*>(this));
    return QObject::qt_metacast(_clname);
}

int UI::UpdateWidgetManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void UI::UpdateWidgetManager::UpdateWidget(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

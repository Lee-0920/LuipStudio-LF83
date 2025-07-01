/****************************************************************************
** Meta object code from reading C++ file 'WaveData.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/Data/WaveData.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WaveData.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__WaveData_t {
    QByteArrayData data[6];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__WaveData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__WaveData_t qt_meta_stringdata_UI__WaveData = {
    {
QT_MOC_LITERAL(0, 0, 12), // "UI::WaveData"
QT_MOC_LITERAL(1, 13, 18), // "UpdaterDataSignals"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "BackItem"
QT_MOC_LITERAL(4, 42, 8), // "NextItem"
QT_MOC_LITERAL(5, 51, 15) // "UpdaterDataSlot"

    },
    "UI::WaveData\0UpdaterDataSignals\0\0"
    "BackItem\0NextItem\0UpdaterDataSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__WaveData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UI::WaveData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WaveData *_t = static_cast<WaveData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdaterDataSignals(); break;
        case 1: _t->BackItem(); break;
        case 2: _t->NextItem(); break;
        case 3: _t->UpdaterDataSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (WaveData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WaveData::UpdaterDataSignals)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject UI::WaveData::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UI__WaveData.data,
      qt_meta_data_UI__WaveData,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::WaveData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::WaveData::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__WaveData.stringdata0))
        return static_cast<void*>(const_cast< WaveData*>(this));
    if (!strcmp(_clname, "IUserChangeNotifiable"))
        return static_cast< IUserChangeNotifiable*>(const_cast< WaveData*>(this));
    if (!strcmp(_clname, "IUpdateWidgetNotifiable"))
        return static_cast< IUpdateWidgetNotifiable*>(const_cast< WaveData*>(this));
    if (!strcmp(_clname, "IResultNotifiable"))
        return static_cast< IResultNotifiable*>(const_cast< WaveData*>(this));
    return QWidget::qt_metacast(_clname);
}

int UI::WaveData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void UI::WaveData::UpdaterDataSignals()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

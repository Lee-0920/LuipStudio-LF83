/****************************************************************************
** Meta object code from reading C++ file 'CalibrateCurve.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/Data/CalibrateCurve.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CalibrateCurve.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__CalibrateCurve_t {
    QByteArrayData data[22];
    char stringdata0[259];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__CalibrateCurve_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__CalibrateCurve_t qt_meta_stringdata_UI__CalibrateCurve = {
    {
QT_MOC_LITERAL(0, 0, 18), // "UI::CalibrateCurve"
QT_MOC_LITERAL(1, 19, 23), // "UpdaterCalibrateSignals"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 6), // "curveB"
QT_MOC_LITERAL(4, 51, 6), // "curveK"
QT_MOC_LITERAL(5, 58, 12), // "SearchResult"
QT_MOC_LITERAL(6, 71, 5), // "ToTop"
QT_MOC_LITERAL(7, 77, 8), // "ToBottom"
QT_MOC_LITERAL(8, 86, 6), // "ToNext"
QT_MOC_LITERAL(9, 93, 6), // "ToBack"
QT_MOC_LITERAL(10, 100, 5), // "ToMap"
QT_MOC_LITERAL(11, 106, 7), // "ToInput"
QT_MOC_LITERAL(12, 114, 16), // "SlotExportButton"
QT_MOC_LITERAL(13, 131, 15), // "SlotClearButton"
QT_MOC_LITERAL(14, 147, 11), // "ChoseMinDay"
QT_MOC_LITERAL(15, 159, 11), // "ChoseMaxDay"
QT_MOC_LITERAL(16, 171, 12), // "ChangeMinDay"
QT_MOC_LITERAL(17, 184, 12), // "ChangeMaxDay"
QT_MOC_LITERAL(18, 197, 20), // "UpdaterCalibrateSlot"
QT_MOC_LITERAL(19, 218, 17), // "DoubleClickedSlot"
QT_MOC_LITERAL(20, 236, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(21, 254, 4) // "item"

    },
    "UI::CalibrateCurve\0UpdaterCalibrateSignals\0"
    "\0curveB\0curveK\0SearchResult\0ToTop\0"
    "ToBottom\0ToNext\0ToBack\0ToMap\0ToInput\0"
    "SlotExportButton\0SlotClearButton\0"
    "ChoseMinDay\0ChoseMaxDay\0ChangeMinDay\0"
    "ChangeMaxDay\0UpdaterCalibrateSlot\0"
    "DoubleClickedSlot\0QTableWidgetItem*\0"
    "item"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__CalibrateCurve[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   99,    2, 0x08 /* Private */,
       6,    0,  100,    2, 0x08 /* Private */,
       7,    0,  101,    2, 0x08 /* Private */,
       8,    0,  102,    2, 0x08 /* Private */,
       9,    0,  103,    2, 0x08 /* Private */,
      10,    0,  104,    2, 0x08 /* Private */,
      11,    0,  105,    2, 0x08 /* Private */,
      12,    0,  106,    2, 0x08 /* Private */,
      13,    0,  107,    2, 0x08 /* Private */,
      14,    0,  108,    2, 0x08 /* Private */,
      15,    0,  109,    2, 0x08 /* Private */,
      16,    0,  110,    2, 0x08 /* Private */,
      17,    0,  111,    2, 0x08 /* Private */,
      18,    2,  112,    2, 0x08 /* Private */,
      19,    1,  117,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    3,    4,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

void UI::CalibrateCurve::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CalibrateCurve *_t = static_cast<CalibrateCurve *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdaterCalibrateSignals((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->SearchResult(); break;
        case 2: _t->ToTop(); break;
        case 3: _t->ToBottom(); break;
        case 4: _t->ToNext(); break;
        case 5: _t->ToBack(); break;
        case 6: _t->ToMap(); break;
        case 7: _t->ToInput(); break;
        case 8: _t->SlotExportButton(); break;
        case 9: _t->SlotClearButton(); break;
        case 10: _t->ChoseMinDay(); break;
        case 11: _t->ChoseMaxDay(); break;
        case 12: _t->ChangeMinDay(); break;
        case 13: _t->ChangeMaxDay(); break;
        case 14: _t->UpdaterCalibrateSlot((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 15: _t->DoubleClickedSlot((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CalibrateCurve::*_t)(float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CalibrateCurve::UpdaterCalibrateSignals)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject UI::CalibrateCurve::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UI__CalibrateCurve.data,
      qt_meta_data_UI__CalibrateCurve,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::CalibrateCurve::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::CalibrateCurve::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__CalibrateCurve.stringdata0))
        return static_cast<void*>(const_cast< CalibrateCurve*>(this));
    if (!strcmp(_clname, "IUserChangeNotifiable"))
        return static_cast< IUserChangeNotifiable*>(const_cast< CalibrateCurve*>(this));
    if (!strcmp(_clname, "IResultNotifiable"))
        return static_cast< IResultNotifiable*>(const_cast< CalibrateCurve*>(this));
    if (!strcmp(_clname, "IUpdateWidgetNotifiable"))
        return static_cast< IUpdateWidgetNotifiable*>(const_cast< CalibrateCurve*>(this));
    return QWidget::qt_metacast(_clname);
}

int UI::CalibrateCurve::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void UI::CalibrateCurve::UpdaterCalibrateSignals(float _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

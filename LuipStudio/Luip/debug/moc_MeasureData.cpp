/****************************************************************************
** Meta object code from reading C++ file 'MeasureData.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/Data/MeasureData.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MeasureData.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__MeasureData_t {
    QByteArrayData data[18];
    char stringdata0[218];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__MeasureData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__MeasureData_t qt_meta_stringdata_UI__MeasureData = {
    {
QT_MOC_LITERAL(0, 0, 15), // "UI::MeasureData"
QT_MOC_LITERAL(1, 16, 18), // "UpdaterDataSignals"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 12), // "SearchResult"
QT_MOC_LITERAL(4, 49, 5), // "ToTop"
QT_MOC_LITERAL(5, 55, 8), // "ToBottom"
QT_MOC_LITERAL(6, 64, 6), // "ToBack"
QT_MOC_LITERAL(7, 71, 6), // "ToNext"
QT_MOC_LITERAL(8, 78, 11), // "ChoseMinDay"
QT_MOC_LITERAL(9, 90, 11), // "ChoseMaxDay"
QT_MOC_LITERAL(10, 102, 12), // "ChangeMinDay"
QT_MOC_LITERAL(11, 115, 12), // "ChangeMaxDay"
QT_MOC_LITERAL(12, 128, 16), // "SlotExportButton"
QT_MOC_LITERAL(13, 145, 15), // "SlotClearButton"
QT_MOC_LITERAL(14, 161, 15), // "UpdaterDataSlot"
QT_MOC_LITERAL(15, 177, 17), // "DoubleClickedSlot"
QT_MOC_LITERAL(16, 195, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(17, 213, 4) // "item"

    },
    "UI::MeasureData\0UpdaterDataSignals\0\0"
    "SearchResult\0ToTop\0ToBottom\0ToBack\0"
    "ToNext\0ChoseMinDay\0ChoseMaxDay\0"
    "ChangeMinDay\0ChangeMaxDay\0SlotExportButton\0"
    "SlotClearButton\0UpdaterDataSlot\0"
    "DoubleClickedSlot\0QTableWidgetItem*\0"
    "item"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__MeasureData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    0,   95,    2, 0x08 /* Private */,
      14,    0,   96,    2, 0x08 /* Private */,
      15,    1,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

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
    QMetaType::Void, 0x80000000 | 16,   17,

       0        // eod
};

void UI::MeasureData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MeasureData *_t = static_cast<MeasureData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdaterDataSignals(); break;
        case 1: _t->SearchResult(); break;
        case 2: _t->ToTop(); break;
        case 3: _t->ToBottom(); break;
        case 4: _t->ToBack(); break;
        case 5: _t->ToNext(); break;
        case 6: _t->ChoseMinDay(); break;
        case 7: _t->ChoseMaxDay(); break;
        case 8: _t->ChangeMinDay(); break;
        case 9: _t->ChangeMaxDay(); break;
        case 10: _t->SlotExportButton(); break;
        case 11: _t->SlotClearButton(); break;
        case 12: _t->UpdaterDataSlot(); break;
        case 13: _t->DoubleClickedSlot((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MeasureData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MeasureData::UpdaterDataSignals)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject UI::MeasureData::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UI__MeasureData.data,
      qt_meta_data_UI__MeasureData,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::MeasureData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::MeasureData::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__MeasureData.stringdata0))
        return static_cast<void*>(const_cast< MeasureData*>(this));
    if (!strcmp(_clname, "IUserChangeNotifiable"))
        return static_cast< IUserChangeNotifiable*>(const_cast< MeasureData*>(this));
    if (!strcmp(_clname, "IResultNotifiable"))
        return static_cast< IResultNotifiable*>(const_cast< MeasureData*>(this));
    return QWidget::qt_metacast(_clname);
}

int UI::MeasureData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void UI::MeasureData::UpdaterDataSignals()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

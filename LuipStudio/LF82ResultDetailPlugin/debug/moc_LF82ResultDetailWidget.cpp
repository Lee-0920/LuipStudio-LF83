/****************************************************************************
** Meta object code from reading C++ file 'LF82ResultDetailWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../LF82ResultDetailWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LF82ResultDetailWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ResultDetail__LF82ResultDetailWidget_t {
    QByteArrayData data[4];
    char stringdata0[61];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ResultDetail__LF82ResultDetailWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ResultDetail__LF82ResultDetailWidget_t qt_meta_stringdata_ResultDetail__LF82ResultDetailWidget = {
    {
QT_MOC_LITERAL(0, 0, 36), // "ResultDetail::LF82ResultDetai..."
QT_MOC_LITERAL(1, 37, 5), // "Close"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 16) // "SlotExportButton"

    },
    "ResultDetail::LF82ResultDetailWidget\0"
    "Close\0\0SlotExportButton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ResultDetail__LF82ResultDetailWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ResultDetail::LF82ResultDetailWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LF82ResultDetailWidget *_t = static_cast<LF82ResultDetailWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Close(); break;
        case 1: _t->SlotExportButton(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ResultDetail::LF82ResultDetailWidget::staticMetaObject = {
    { &MeasureResultDetailWidget::staticMetaObject, qt_meta_stringdata_ResultDetail__LF82ResultDetailWidget.data,
      qt_meta_data_ResultDetail__LF82ResultDetailWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ResultDetail::LF82ResultDetailWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ResultDetail::LF82ResultDetailWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ResultDetail__LF82ResultDetailWidget.stringdata0))
        return static_cast<void*>(const_cast< LF82ResultDetailWidget*>(this));
    return MeasureResultDetailWidget::qt_metacast(_clname);
}

int ResultDetail::LF82ResultDetailWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MeasureResultDetailWidget::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE

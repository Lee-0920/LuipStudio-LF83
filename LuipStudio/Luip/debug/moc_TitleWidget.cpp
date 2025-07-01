/****************************************************************************
** Meta object code from reading C++ file 'TitleWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/Frame/TitleWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TitleWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UI__TitleWidget_t {
    QByteArrayData data[12];
    char stringdata0[182];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI__TitleWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI__TitleWidget_t qt_meta_stringdata_UI__TitleWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "UI::TitleWidget"
QT_MOC_LITERAL(1, 16, 8), // "TurnPage"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 11), // "currentPage"
QT_MOC_LITERAL(4, 38, 16), // "CreatContextMenu"
QT_MOC_LITERAL(5, 55, 19), // "ChangeUserInterface"
QT_MOC_LITERAL(6, 75, 17), // "ScreenAlreadyLock"
QT_MOC_LITERAL(7, 93, 12), // "SlotTurnPage"
QT_MOC_LITERAL(8, 106, 20), // "SlotCreatContextMenu"
QT_MOC_LITERAL(9, 127, 14), // "slotChangeUser"
QT_MOC_LITERAL(10, 142, 18), // "slotModifyPassword"
QT_MOC_LITERAL(11, 161, 20) // "slotUserCancellation"

    },
    "UI::TitleWidget\0TurnPage\0\0currentPage\0"
    "CreatContextMenu\0ChangeUserInterface\0"
    "ScreenAlreadyLock\0SlotTurnPage\0"
    "SlotCreatContextMenu\0slotChangeUser\0"
    "slotModifyPassword\0slotUserCancellation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI__TitleWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    0,   62,    2, 0x06 /* Public */,
       5,    0,   63,    2, 0x06 /* Public */,
       6,    0,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   65,    2, 0x0a /* Public */,
       8,    0,   68,    2, 0x0a /* Public */,
       9,    0,   69,    2, 0x0a /* Public */,
      10,    0,   70,    2, 0x0a /* Public */,
      11,    0,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UI::TitleWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TitleWidget *_t = static_cast<TitleWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->TurnPage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->CreatContextMenu(); break;
        case 2: _t->ChangeUserInterface(); break;
        case 3: _t->ScreenAlreadyLock(); break;
        case 4: _t->SlotTurnPage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->SlotCreatContextMenu(); break;
        case 6: _t->slotChangeUser(); break;
        case 7: _t->slotModifyPassword(); break;
        case 8: _t->slotUserCancellation(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TitleWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitleWidget::TurnPage)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TitleWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitleWidget::CreatContextMenu)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TitleWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitleWidget::ChangeUserInterface)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (TitleWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitleWidget::ScreenAlreadyLock)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject UI::TitleWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UI__TitleWidget.data,
      qt_meta_data_UI__TitleWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UI::TitleWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::TitleWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UI__TitleWidget.stringdata0))
        return static_cast<void*>(const_cast< TitleWidget*>(this));
    if (!strcmp(_clname, "ILockScreenNotifiable"))
        return static_cast< ILockScreenNotifiable*>(const_cast< TitleWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int UI::TitleWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void UI::TitleWidget::TurnPage(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UI::TitleWidget::CreatContextMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void UI::TitleWidget::ChangeUserInterface()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void UI::TitleWidget::ScreenAlreadyLock()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

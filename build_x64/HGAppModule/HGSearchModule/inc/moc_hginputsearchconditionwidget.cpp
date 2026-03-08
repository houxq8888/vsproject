/****************************************************************************
** Meta object code from reading C++ file 'hginputsearchconditionwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../HGAppModule/HGSearchModule/inc/hginputsearchconditionwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hginputsearchconditionwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HGInputSearchConditionWidget_t {
    QByteArrayData data[13];
    char stringdata0[171];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HGInputSearchConditionWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HGInputSearchConditionWidget_t qt_meta_stringdata_HGInputSearchConditionWidget = {
    {
QT_MOC_LITERAL(0, 0, 28), // "HGInputSearchConditionWidget"
QT_MOC_LITERAL(1, 29, 14), // "signalTimeFrom"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 4), // "text"
QT_MOC_LITERAL(4, 50, 12), // "signalTimeTo"
QT_MOC_LITERAL(5, 63, 13), // "signalKeyWord"
QT_MOC_LITERAL(6, 77, 12), // "signalSearch"
QT_MOC_LITERAL(7, 90, 17), // "signalClearSearch"
QT_MOC_LITERAL(8, 108, 12), // "slotTimeFrom"
QT_MOC_LITERAL(9, 121, 10), // "slotTimeTo"
QT_MOC_LITERAL(10, 132, 11), // "slotKeyWord"
QT_MOC_LITERAL(11, 144, 10), // "slotSearch"
QT_MOC_LITERAL(12, 155, 15) // "slotClearSearch"

    },
    "HGInputSearchConditionWidget\0"
    "signalTimeFrom\0\0text\0signalTimeTo\0"
    "signalKeyWord\0signalSearch\0signalClearSearch\0"
    "slotTimeFrom\0slotTimeTo\0slotKeyWord\0"
    "slotSearch\0slotClearSearch"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HGInputSearchConditionWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       5,    1,   70,    2, 0x06 /* Public */,
       6,    0,   73,    2, 0x06 /* Public */,
       7,    0,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   75,    2, 0x08 /* Private */,
       9,    1,   78,    2, 0x08 /* Private */,
      10,    1,   81,    2, 0x08 /* Private */,
      11,    0,   84,    2, 0x08 /* Private */,
      12,    0,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HGInputSearchConditionWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HGInputSearchConditionWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signalTimeFrom((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->signalTimeTo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->signalKeyWord((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->signalSearch(); break;
        case 4: _t->signalClearSearch(); break;
        case 5: _t->slotTimeFrom((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->slotTimeTo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->slotKeyWord((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->slotSearch(); break;
        case 9: _t->slotClearSearch(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HGInputSearchConditionWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HGInputSearchConditionWidget::signalTimeFrom)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HGInputSearchConditionWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HGInputSearchConditionWidget::signalTimeTo)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HGInputSearchConditionWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HGInputSearchConditionWidget::signalKeyWord)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (HGInputSearchConditionWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HGInputSearchConditionWidget::signalSearch)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (HGInputSearchConditionWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HGInputSearchConditionWidget::signalClearSearch)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HGInputSearchConditionWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_HGInputSearchConditionWidget.data,
    qt_meta_data_HGInputSearchConditionWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HGInputSearchConditionWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HGInputSearchConditionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HGInputSearchConditionWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HGInputSearchConditionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void HGInputSearchConditionWidget::signalTimeFrom(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HGInputSearchConditionWidget::signalTimeTo(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HGInputSearchConditionWidget::signalKeyWord(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HGInputSearchConditionWidget::signalSearch()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void HGInputSearchConditionWidget::signalClearSearch()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

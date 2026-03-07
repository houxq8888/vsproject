/****************************************************************************
** Meta object code from reading C++ file 'mykeyboard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../HGAppModule/HGBaseAppModule/inc/mykeyboard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mykeyboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KBTimeWithHourEdit_t {
    QByteArrayData data[5];
    char stringdata0[53];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KBTimeWithHourEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KBTimeWithHourEdit_t qt_meta_stringdata_KBTimeWithHourEdit = {
    {
QT_MOC_LITERAL(0, 0, 18), // "KBTimeWithHourEdit"
QT_MOC_LITERAL(1, 19, 16), // "MykeyboardSignal"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 11), // "std::string"
QT_MOC_LITERAL(4, 49, 3) // "str"

    },
    "KBTimeWithHourEdit\0MykeyboardSignal\0"
    "\0std::string\0str"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KBTimeWithHourEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void KBTimeWithHourEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KBTimeWithHourEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->MykeyboardSignal((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KBTimeWithHourEdit::*)(const std::string & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KBTimeWithHourEdit::MykeyboardSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KBTimeWithHourEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_KBTimeWithHourEdit.data,
    qt_meta_data_KBTimeWithHourEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KBTimeWithHourEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KBTimeWithHourEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KBTimeWithHourEdit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int KBTimeWithHourEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void KBTimeWithHourEdit::MykeyboardSignal(const std::string & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_KBTimeEdit_t {
    QByteArrayData data[4];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KBTimeEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KBTimeEdit_t qt_meta_stringdata_KBTimeEdit = {
    {
QT_MOC_LITERAL(0, 0, 10), // "KBTimeEdit"
QT_MOC_LITERAL(1, 11, 16), // "MykeyboardSignal"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3) // "str"

    },
    "KBTimeEdit\0MykeyboardSignal\0\0str"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KBTimeEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void KBTimeEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KBTimeEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->MykeyboardSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KBTimeEdit::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KBTimeEdit::MykeyboardSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KBTimeEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_KBTimeEdit.data,
    qt_meta_data_KBTimeEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KBTimeEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KBTimeEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KBTimeEdit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int KBTimeEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void KBTimeEdit::MykeyboardSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_KBUIntEdit_t {
    QByteArrayData data[4];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KBUIntEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KBUIntEdit_t qt_meta_stringdata_KBUIntEdit = {
    {
QT_MOC_LITERAL(0, 0, 10), // "KBUIntEdit"
QT_MOC_LITERAL(1, 11, 16), // "MykeyboardSignal"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3) // "str"

    },
    "KBUIntEdit\0MykeyboardSignal\0\0str"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KBUIntEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void KBUIntEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KBUIntEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->MykeyboardSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KBUIntEdit::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KBUIntEdit::MykeyboardSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KBUIntEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_KBUIntEdit.data,
    qt_meta_data_KBUIntEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KBUIntEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KBUIntEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KBUIntEdit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int KBUIntEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void KBUIntEdit::MykeyboardSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_KBIntEdit_t {
    QByteArrayData data[3];
    char stringdata0[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KBIntEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KBIntEdit_t qt_meta_stringdata_KBIntEdit = {
    {
QT_MOC_LITERAL(0, 0, 9), // "KBIntEdit"
QT_MOC_LITERAL(1, 10, 16), // "MykeyboardSignal"
QT_MOC_LITERAL(2, 27, 0) // ""

    },
    "KBIntEdit\0MykeyboardSignal\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KBIntEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void KBIntEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KBIntEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->MykeyboardSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KBIntEdit::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KBIntEdit::MykeyboardSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KBIntEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_KBIntEdit.data,
    qt_meta_data_KBIntEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KBIntEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KBIntEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KBIntEdit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int KBIntEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void KBIntEdit::MykeyboardSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_KBFLoatEdit_t {
    QByteArrayData data[3];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KBFLoatEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KBFLoatEdit_t qt_meta_stringdata_KBFLoatEdit = {
    {
QT_MOC_LITERAL(0, 0, 11), // "KBFLoatEdit"
QT_MOC_LITERAL(1, 12, 16), // "MykeyboardSignal"
QT_MOC_LITERAL(2, 29, 0) // ""

    },
    "KBFLoatEdit\0MykeyboardSignal\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KBFLoatEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void KBFLoatEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KBFLoatEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->MykeyboardSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KBFLoatEdit::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KBFLoatEdit::MykeyboardSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KBFLoatEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_KBFLoatEdit.data,
    qt_meta_data_KBFLoatEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KBFLoatEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KBFLoatEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KBFLoatEdit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int KBFLoatEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void KBFLoatEdit::MykeyboardSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_KBCharEdit_t {
    QByteArrayData data[4];
    char stringdata0[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KBCharEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KBCharEdit_t qt_meta_stringdata_KBCharEdit = {
    {
QT_MOC_LITERAL(0, 0, 10), // "KBCharEdit"
QT_MOC_LITERAL(1, 11, 16), // "MykeyboardSignal"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 15) // "button_capsslot"

    },
    "KBCharEdit\0MykeyboardSignal\0\0"
    "button_capsslot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KBCharEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   27,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void KBCharEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KBCharEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->MykeyboardSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->button_capsslot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KBCharEdit::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KBCharEdit::MykeyboardSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KBCharEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_KBCharEdit.data,
    qt_meta_data_KBCharEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KBCharEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KBCharEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KBCharEdit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int KBCharEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void KBCharEdit::MykeyboardSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

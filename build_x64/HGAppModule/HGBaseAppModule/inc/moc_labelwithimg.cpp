/****************************************************************************
** Meta object code from reading C++ file 'labelwithimg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../HGAppModule/HGBaseAppModule/inc/labelwithimg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'labelwithimg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LabelWithImg_t {
    QByteArrayData data[8];
    char stringdata0[92];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LabelWithImg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LabelWithImg_t qt_meta_stringdata_LabelWithImg = {
    {
QT_MOC_LITERAL(0, 0, 12), // "LabelWithImg"
QT_MOC_LITERAL(1, 13, 15), // "clickLeftButton"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 13), // "clickLeftName"
QT_MOC_LITERAL(4, 44, 11), // "std::string"
QT_MOC_LITERAL(5, 56, 13), // "clickImgLabel"
QT_MOC_LITERAL(6, 70, 12), // "createModule"
QT_MOC_LITERAL(7, 83, 8) // "clickImg"

    },
    "LabelWithImg\0clickLeftButton\0\0"
    "clickLeftName\0std::string\0clickImgLabel\0"
    "createModule\0clickImg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LabelWithImg[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,
       5,    0,   43,    2, 0x06 /* Public */,
       6,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   47,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void LabelWithImg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LabelWithImg *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clickLeftButton(); break;
        case 1: _t->clickLeftName((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 2: _t->clickImgLabel(); break;
        case 3: _t->createModule((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 4: _t->clickImg(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LabelWithImg::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LabelWithImg::clickLeftButton)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LabelWithImg::*)(std::string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LabelWithImg::clickLeftName)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LabelWithImg::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LabelWithImg::clickImgLabel)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (LabelWithImg::*)(std::string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LabelWithImg::createModule)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LabelWithImg::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_LabelWithImg.data,
    qt_meta_data_LabelWithImg,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LabelWithImg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LabelWithImg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LabelWithImg.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LabelWithImg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void LabelWithImg::clickLeftButton()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void LabelWithImg::clickLeftName(std::string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LabelWithImg::clickImgLabel()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void LabelWithImg::createModule(std::string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

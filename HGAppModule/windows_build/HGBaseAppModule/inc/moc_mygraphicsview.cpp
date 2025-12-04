/****************************************************************************
** Meta object code from reading C++ file 'mygraphicsview.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../HGBaseAppModule/inc/mygraphicsview.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mygraphicsview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN14MyGraphicsViewE_t {};
} // unnamed namespace

template <> constexpr inline auto MyGraphicsView::qt_create_metaobjectdata<qt_meta_tag_ZN14MyGraphicsViewE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MyGraphicsView",
        "mouseDoubleClick",
        "",
        "changezoom",
        "pixgridsignal",
        "subpixgridsignal",
        "startsignal",
        "simulatesignal",
        "leftdoubsignal",
        "getScenePos",
        "x",
        "y",
        "Pointer_slot",
        "Pan_slot",
        "Zoomin_slot",
        "Zoomout_slot",
        "Fitimage_slot",
        "Zoom_slot",
        "pixelgrid_slot",
        "SubpixelGrid_slot"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'mouseDoubleClick'
        QtMocHelpers::SignalData<void(QPoint)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QPoint, 2 },
        }}),
        // Signal 'changezoom'
        QtMocHelpers::SignalData<void(bool, int)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 2 }, { QMetaType::Int, 2 },
        }}),
        // Signal 'pixgridsignal'
        QtMocHelpers::SignalData<void(bool, int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 2 }, { QMetaType::Int, 2 },
        }}),
        // Signal 'subpixgridsignal'
        QtMocHelpers::SignalData<void(bool, int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 2 }, { QMetaType::Int, 2 },
        }}),
        // Signal 'startsignal'
        QtMocHelpers::SignalData<void(int, bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 2 }, { QMetaType::Bool, 2 },
        }}),
        // Signal 'simulatesignal'
        QtMocHelpers::SignalData<void(int, bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 2 }, { QMetaType::Bool, 2 },
        }}),
        // Signal 'leftdoubsignal'
        QtMocHelpers::SignalData<void(int, bool)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 2 }, { QMetaType::Bool, 2 },
        }}),
        // Signal 'getScenePos'
        QtMocHelpers::SignalData<void(int, int)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 10 }, { QMetaType::Int, 11 },
        }}),
        // Slot 'Pointer_slot'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'Pan_slot'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'Zoomin_slot'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'Zoomout_slot'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'Fitimage_slot'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'Zoom_slot'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'pixelgrid_slot'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'SubpixelGrid_slot'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessProtected, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MyGraphicsView, qt_meta_tag_ZN14MyGraphicsViewE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MyGraphicsView::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14MyGraphicsViewE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14MyGraphicsViewE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14MyGraphicsViewE_t>.metaTypes,
    nullptr
} };

void MyGraphicsView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MyGraphicsView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->mouseDoubleClick((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 1: _t->changezoom((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->pixgridsignal((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->subpixgridsignal((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->startsignal((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 5: _t->simulatesignal((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 6: _t->leftdoubsignal((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 7: _t->getScenePos((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->Pointer_slot(); break;
        case 9: _t->Pan_slot(); break;
        case 10: _t->Zoomin_slot(); break;
        case 11: _t->Zoomout_slot(); break;
        case 12: _t->Fitimage_slot(); break;
        case 13: _t->Zoom_slot(); break;
        case 14: _t->pixelgrid_slot(); break;
        case 15: _t->SubpixelGrid_slot(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MyGraphicsView::*)(QPoint )>(_a, &MyGraphicsView::mouseDoubleClick, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MyGraphicsView::*)(bool , int )>(_a, &MyGraphicsView::changezoom, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (MyGraphicsView::*)(bool , int )>(_a, &MyGraphicsView::pixgridsignal, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (MyGraphicsView::*)(bool , int )>(_a, &MyGraphicsView::subpixgridsignal, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (MyGraphicsView::*)(int , bool )>(_a, &MyGraphicsView::startsignal, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (MyGraphicsView::*)(int , bool )>(_a, &MyGraphicsView::simulatesignal, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (MyGraphicsView::*)(int , bool )>(_a, &MyGraphicsView::leftdoubsignal, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (MyGraphicsView::*)(int , int )>(_a, &MyGraphicsView::getScenePos, 7))
            return;
    }
}

const QMetaObject *MyGraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14MyGraphicsViewE_t>.strings))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int MyGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void MyGraphicsView::mouseDoubleClick(QPoint _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void MyGraphicsView::changezoom(bool _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void MyGraphicsView::pixgridsignal(bool _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void MyGraphicsView::subpixgridsignal(bool _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void MyGraphicsView::startsignal(int _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void MyGraphicsView::simulatesignal(int _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void MyGraphicsView::leftdoubsignal(int _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}

// SIGNAL 7
void MyGraphicsView::getScenePos(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2);
}
QT_WARNING_POP

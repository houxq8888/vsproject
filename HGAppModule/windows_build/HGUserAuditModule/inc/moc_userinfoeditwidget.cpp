/****************************************************************************
** Meta object code from reading C++ file 'userinfoeditwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../HGUserAuditModule/inc/userinfoeditwidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userinfoeditwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN18UserInfoEditWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto UserInfoEditWidget::qt_create_metaobjectdata<qt_meta_tag_ZN18UserInfoEditWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "UserInfoEditWidget",
        "signalUserNo",
        "",
        "std::string",
        "signalBack",
        "signalUserInfo",
        "std::map<std::string,std::string>",
        "updateAuthority",
        "clickBack",
        "slotOk",
        "onAccountComboBoxChanged",
        "onUserNoEditChanged",
        "onPasswdEditChanged",
        "slotEyeClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalUserNo'
        QtMocHelpers::SignalData<void(std::string)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 2 },
        }}),
        // Signal 'signalBack'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signalUserInfo'
        QtMocHelpers::SignalData<void(const std::map<std::string,std::string> &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 2 },
        }}),
        // Signal 'updateAuthority'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'clickBack'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotOk'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onAccountComboBoxChanged'
        QtMocHelpers::SlotData<void(int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'onUserNoEditChanged'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onPasswdEditChanged'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotEyeClicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<UserInfoEditWidget, qt_meta_tag_ZN18UserInfoEditWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject UserInfoEditWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18UserInfoEditWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18UserInfoEditWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18UserInfoEditWidgetE_t>.metaTypes,
    nullptr
} };

void UserInfoEditWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<UserInfoEditWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalUserNo((*reinterpret_cast< std::add_pointer_t<std::string>>(_a[1]))); break;
        case 1: _t->signalBack(); break;
        case 2: _t->signalUserInfo((*reinterpret_cast< std::add_pointer_t<std::map<std::string,std::string>>>(_a[1]))); break;
        case 3: _t->updateAuthority(); break;
        case 4: _t->clickBack(); break;
        case 5: _t->slotOk(); break;
        case 6: _t->onAccountComboBoxChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->onUserNoEditChanged(); break;
        case 8: _t->onPasswdEditChanged(); break;
        case 9: _t->slotEyeClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (UserInfoEditWidget::*)(std::string )>(_a, &UserInfoEditWidget::signalUserNo, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (UserInfoEditWidget::*)()>(_a, &UserInfoEditWidget::signalBack, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (UserInfoEditWidget::*)(const std::map<std::string,std::string> & )>(_a, &UserInfoEditWidget::signalUserInfo, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (UserInfoEditWidget::*)()>(_a, &UserInfoEditWidget::updateAuthority, 3))
            return;
    }
}

const QMetaObject *UserInfoEditWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserInfoEditWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18UserInfoEditWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int UserInfoEditWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void UserInfoEditWidget::signalUserNo(std::string _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void UserInfoEditWidget::signalBack()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void UserInfoEditWidget::signalUserInfo(const std::map<std::string,std::string> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void UserInfoEditWidget::updateAuthority()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP

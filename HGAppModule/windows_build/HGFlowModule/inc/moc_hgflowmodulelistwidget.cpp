/****************************************************************************
** Meta object code from reading C++ file 'hgflowmodulelistwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../HGFlowModule/inc/hgflowmodulelistwidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hgflowmodulelistwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN22HGFlowModuleListWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto HGFlowModuleListWidget::qt_create_metaobjectdata<qt_meta_tag_ZN22HGFlowModuleListWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "HGFlowModuleListWidget",
        "createModule",
        "",
        "std::string",
        "slotClickGetSample",
        "slotClickQuantity",
        "slotClickDilution",
        "slotClickWash",
        "slotClickBlank",
        "slotClickWait",
        "slotClickOffset",
        "slotClickInjectReagent",
        "slotClickDetect",
        "slotClickRejectLiquid",
        "slotClickHoldSample",
        "slotClickJudge",
        "slotClickAddMedication",
        "slotClickLoop",
        "slotCreateModule",
        "slotClickBalance",
        "slotClickInjectSample",
        "slotClickSwirl"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'createModule'
        QtMocHelpers::SignalData<void(std::string)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 2 },
        }}),
        // Slot 'slotClickGetSample'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickQuantity'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickDilution'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickWash'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickBlank'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickWait'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickOffset'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickInjectReagent'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickDetect'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickRejectLiquid'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickHoldSample'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickJudge'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickAddMedication'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickLoop'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotCreateModule'
        QtMocHelpers::SlotData<void(std::string)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 2 },
        }}),
        // Slot 'slotClickBalance'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickInjectSample'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotClickSwirl'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<HGFlowModuleListWidget, qt_meta_tag_ZN22HGFlowModuleListWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject HGFlowModuleListWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22HGFlowModuleListWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22HGFlowModuleListWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN22HGFlowModuleListWidgetE_t>.metaTypes,
    nullptr
} };

void HGFlowModuleListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<HGFlowModuleListWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->createModule((*reinterpret_cast< std::add_pointer_t<std::string>>(_a[1]))); break;
        case 1: _t->slotClickGetSample(); break;
        case 2: _t->slotClickQuantity(); break;
        case 3: _t->slotClickDilution(); break;
        case 4: _t->slotClickWash(); break;
        case 5: _t->slotClickBlank(); break;
        case 6: _t->slotClickWait(); break;
        case 7: _t->slotClickOffset(); break;
        case 8: _t->slotClickInjectReagent(); break;
        case 9: _t->slotClickDetect(); break;
        case 10: _t->slotClickRejectLiquid(); break;
        case 11: _t->slotClickHoldSample(); break;
        case 12: _t->slotClickJudge(); break;
        case 13: _t->slotClickAddMedication(); break;
        case 14: _t->slotClickLoop(); break;
        case 15: _t->slotCreateModule((*reinterpret_cast< std::add_pointer_t<std::string>>(_a[1]))); break;
        case 16: _t->slotClickBalance(); break;
        case 17: _t->slotClickInjectSample(); break;
        case 18: _t->slotClickSwirl(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (HGFlowModuleListWidget::*)(std::string )>(_a, &HGFlowModuleListWidget::createModule, 0))
            return;
    }
}

const QMetaObject *HGFlowModuleListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HGFlowModuleListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22HGFlowModuleListWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HGFlowModuleListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void HGFlowModuleListWidget::createModule(std::string _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
QT_WARNING_POP

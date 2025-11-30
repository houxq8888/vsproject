/****************************************************************************
** Meta object code from reading C++ file 'camerarecognizeplugindetector.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../HGCameraRecognizeModule/inc/camerarecognizeplugindetector.h"
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camerarecognizeplugindetector.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN29CameraRecognizePluginDetectorE_t {};
} // unnamed namespace

template <> constexpr inline auto CameraRecognizePluginDetector::qt_create_metaobjectdata<qt_meta_tag_ZN29CameraRecognizePluginDetectorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CameraRecognizePluginDetector",
        "sendMsg2Manager",
        "",
        "recvMsgFromUI"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sendMsg2Manager'
        QtMocHelpers::SignalData<void(QString, QString, QString)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 }, { QMetaType::QString, 2 }, { QMetaType::QString, 2 },
        }}),
        // Slot 'recvMsgFromUI'
        QtMocHelpers::SlotData<void(int)>(3, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CameraRecognizePluginDetector, qt_meta_tag_ZN29CameraRecognizePluginDetectorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CameraRecognizePluginDetector::staticMetaObject = { {
    QMetaObject::SuperData::link<PluginInterface::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN29CameraRecognizePluginDetectorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN29CameraRecognizePluginDetectorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN29CameraRecognizePluginDetectorE_t>.metaTypes,
    nullptr
} };

void CameraRecognizePluginDetector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CameraRecognizePluginDetector *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sendMsg2Manager((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 1: _t->recvMsgFromUI((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CameraRecognizePluginDetector::*)(QString , QString , QString )>(_a, &CameraRecognizePluginDetector::sendMsg2Manager, 0))
            return;
    }
}

const QMetaObject *CameraRecognizePluginDetector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraRecognizePluginDetector::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN29CameraRecognizePluginDetectorE_t>.strings))
        return static_cast<void*>(this);
    return PluginInterface::qt_metacast(_clname);
}

int CameraRecognizePluginDetector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PluginInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CameraRecognizePluginDetector::sendMsg2Manager(QString _t1, QString _t2, QString _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3);
}

#ifdef QT_MOC_EXPORT_PLUGIN_V2
static constexpr unsigned char qt_pluginMetaDataV2_CameraRecognizePluginDetector[] = {
    0xbf, 
    // "IID"
    0x02,  0x78,  0x28,  'o',  'r',  'g',  '.',  'c', 
    's',  'm',  '.',  'e',  'm',  'b',  'e',  'd', 
    '.',  'p',  'l',  'u',  'g',  'i',  'n',  '.', 
    'p',  'l',  'u',  'g',  'i',  'n',  'I',  'n', 
    't',  'e',  'r',  'f',  'a',  'c',  'e',  '.', 
    '1',  '.',  '0', 
    // "className"
    0x03,  0x78,  0x1d,  'C',  'a',  'm',  'e',  'r', 
    'a',  'R',  'e',  'c',  'o',  'g',  'n',  'i', 
    'z',  'e',  'P',  'l',  'u',  'g',  'i',  'n', 
    'D',  'e',  't',  'e',  'c',  't',  'o',  'r', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN_V2(CameraRecognizePluginDetector, CameraRecognizePluginDetector, qt_pluginMetaDataV2_CameraRecognizePluginDetector)
#else
QT_PLUGIN_METADATA_SECTION
Q_CONSTINIT static constexpr unsigned char qt_pluginMetaData_CameraRecognizePluginDetector[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x78,  0x28,  'o',  'r',  'g',  '.',  'c', 
    's',  'm',  '.',  'e',  'm',  'b',  'e',  'd', 
    '.',  'p',  'l',  'u',  'g',  'i',  'n',  '.', 
    'p',  'l',  'u',  'g',  'i',  'n',  'I',  'n', 
    't',  'e',  'r',  'f',  'a',  'c',  'e',  '.', 
    '1',  '.',  '0', 
    // "className"
    0x03,  0x78,  0x1d,  'C',  'a',  'm',  'e',  'r', 
    'a',  'R',  'e',  'c',  'o',  'g',  'n',  'i', 
    'z',  'e',  'P',  'l',  'u',  'g',  'i',  'n', 
    'D',  'e',  't',  'e',  'c',  't',  'o',  'r', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(CameraRecognizePluginDetector, CameraRecognizePluginDetector)
#endif  // QT_MOC_EXPORT_PLUGIN_V2

QT_WARNING_POP

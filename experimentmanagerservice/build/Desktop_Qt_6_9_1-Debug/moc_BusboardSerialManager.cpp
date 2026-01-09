/****************************************************************************
** Meta object code from reading C++ file 'BusboardSerialManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/BusboardSerialManager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BusboardSerialManager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN21BusboardSerialManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto BusboardSerialManager::qt_create_metaobjectdata<qt_meta_tag_ZN21BusboardSerialManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BusboardSerialManager",
        "sgn_updateCell",
        "",
        "Cell&",
        "cell",
        "sgn_presenceUpdate",
        "slotIndex",
        "isPresent",
        "serialRecieved"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sgn_updateCell'
        QtMocHelpers::SignalData<void(Cell &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'sgn_presenceUpdate'
        QtMocHelpers::SignalData<void(int, bool)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Bool, 7 },
        }}),
        // Slot 'serialRecieved'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BusboardSerialManager, qt_meta_tag_ZN21BusboardSerialManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BusboardSerialManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21BusboardSerialManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21BusboardSerialManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN21BusboardSerialManagerE_t>.metaTypes,
    nullptr
} };

void BusboardSerialManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BusboardSerialManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sgn_updateCell((*reinterpret_cast< std::add_pointer_t<Cell&>>(_a[1]))); break;
        case 1: _t->sgn_presenceUpdate((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 2: _t->serialRecieved(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BusboardSerialManager::*)(Cell & )>(_a, &BusboardSerialManager::sgn_updateCell, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BusboardSerialManager::*)(int , bool )>(_a, &BusboardSerialManager::sgn_presenceUpdate, 1))
            return;
    }
}

const QMetaObject *BusboardSerialManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BusboardSerialManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21BusboardSerialManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BusboardSerialManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void BusboardSerialManager::sgn_updateCell(Cell & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void BusboardSerialManager::sgn_presenceUpdate(int _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}
QT_WARNING_POP

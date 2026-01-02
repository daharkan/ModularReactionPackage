/****************************************************************************
** Meta object code from reading C++ file 'uiHomeWidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../uiHomeWidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uiHomeWidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10HomeWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto HomeWidget::qt_create_metaobjectdata<qt_meta_tag_ZN10HomeWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "HomeWidget",
        "sgn_openReactorWidget",
        "",
        "sgn_openExpManagerWidget",
        "sgn_openUserManagerWidget",
        "sgn_openOptionsWidget",
        "sgn_closeApp",
        "reactorClicked",
        "expManagerClicked",
        "userManagerClicked",
        "optionsClicked",
        "closeClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sgn_openReactorWidget'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sgn_openExpManagerWidget'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sgn_openUserManagerWidget'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sgn_openOptionsWidget'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sgn_closeApp'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'reactorClicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'expManagerClicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'userManagerClicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'optionsClicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'closeClicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<HomeWidget, qt_meta_tag_ZN10HomeWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject HomeWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10HomeWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10HomeWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10HomeWidgetE_t>.metaTypes,
    nullptr
} };

void HomeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<HomeWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sgn_openReactorWidget(); break;
        case 1: _t->sgn_openExpManagerWidget(); break;
        case 2: _t->sgn_openUserManagerWidget(); break;
        case 3: _t->sgn_openOptionsWidget(); break;
        case 4: _t->sgn_closeApp(); break;
        case 5: _t->reactorClicked(); break;
        case 6: _t->expManagerClicked(); break;
        case 7: _t->userManagerClicked(); break;
        case 8: _t->optionsClicked(); break;
        case 9: _t->closeClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (HomeWidget::*)()>(_a, &HomeWidget::sgn_openReactorWidget, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (HomeWidget::*)()>(_a, &HomeWidget::sgn_openExpManagerWidget, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (HomeWidget::*)()>(_a, &HomeWidget::sgn_openUserManagerWidget, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (HomeWidget::*)()>(_a, &HomeWidget::sgn_openOptionsWidget, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (HomeWidget::*)()>(_a, &HomeWidget::sgn_closeApp, 4))
            return;
    }
}

const QMetaObject *HomeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HomeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10HomeWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HomeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void HomeWidget::sgn_openReactorWidget()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HomeWidget::sgn_openExpManagerWidget()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HomeWidget::sgn_openUserManagerWidget()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void HomeWidget::sgn_openOptionsWidget()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void HomeWidget::sgn_closeApp()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP

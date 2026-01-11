/****************************************************************************
** Meta object code from reading C++ file 'OnlyCellSerialManager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../OnlyCellSerialManager.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OnlyCellSerialManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.2. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSOnlyCellSerialManagerENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSOnlyCellSerialManagerENDCLASS = QtMocHelpers::stringData(
    "OnlyCellSerialManager",
    "sgn_updateCell",
    "",
    "Cell&",
    "cell",
    "sgn_updatePIDParameters",
    "heaterPerc",
    "peltierPerc",
    "serialRecieved"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSOnlyCellSerialManagerENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[22];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[5];
    char stringdata5[24];
    char stringdata6[11];
    char stringdata7[12];
    char stringdata8[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSOnlyCellSerialManagerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSOnlyCellSerialManagerENDCLASS_t qt_meta_stringdata_CLASSOnlyCellSerialManagerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 21),  // "OnlyCellSerialManager"
        QT_MOC_LITERAL(22, 14),  // "sgn_updateCell"
        QT_MOC_LITERAL(37, 0),  // ""
        QT_MOC_LITERAL(38, 5),  // "Cell&"
        QT_MOC_LITERAL(44, 4),  // "cell"
        QT_MOC_LITERAL(49, 23),  // "sgn_updatePIDParameters"
        QT_MOC_LITERAL(73, 10),  // "heaterPerc"
        QT_MOC_LITERAL(84, 11),  // "peltierPerc"
        QT_MOC_LITERAL(96, 14)   // "serialRecieved"
    },
    "OnlyCellSerialManager",
    "sgn_updateCell",
    "",
    "Cell&",
    "cell",
    "sgn_updatePIDParameters",
    "heaterPerc",
    "peltierPerc",
    "serialRecieved"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSOnlyCellSerialManagerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    1 /* Public */,
       5,    2,   35,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   40,    2, 0x08,    6 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject OnlyCellSerialManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSOnlyCellSerialManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSOnlyCellSerialManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSOnlyCellSerialManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OnlyCellSerialManager, std::true_type>,
        // method 'sgn_updateCell'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Cell &, std::false_type>,
        // method 'sgn_updatePIDParameters'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'serialRecieved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void OnlyCellSerialManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OnlyCellSerialManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sgn_updateCell((*reinterpret_cast< std::add_pointer_t<Cell&>>(_a[1]))); break;
        case 1: _t->sgn_updatePIDParameters((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->serialRecieved(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OnlyCellSerialManager::*)(Cell & );
            if (_t _q_method = &OnlyCellSerialManager::sgn_updateCell; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OnlyCellSerialManager::*)(int , int );
            if (_t _q_method = &OnlyCellSerialManager::sgn_updatePIDParameters; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *OnlyCellSerialManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OnlyCellSerialManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSOnlyCellSerialManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OnlyCellSerialManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void OnlyCellSerialManager::sgn_updateCell(Cell & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OnlyCellSerialManager::sgn_updatePIDParameters(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP

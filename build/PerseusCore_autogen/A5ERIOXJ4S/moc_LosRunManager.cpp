/****************************************************************************
** Meta object code from reading C++ file 'LosRunManager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/core/LosRunner/LosRunManager/LosRunManager.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LosRunManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_LosCore__LosRunManager_t {
    uint offsetsAndSizes[18];
    char stringdata0[23];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[47];
    char stringdata4[43];
    char stringdata5[5];
    char stringdata6[17];
    char stringdata7[8];
    char stringdata8[5];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_LosCore__LosRunManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_LosCore__LosRunManager_t qt_meta_stringdata_LosCore__LosRunManager = {
    {
        QT_MOC_LITERAL(0, 22),  // "LosCore::LosRunManager"
        QT_MOC_LITERAL(23, 16),  // "onToolChainReady"
        QT_MOC_LITERAL(40, 0),  // ""
        QT_MOC_LITERAL(41, 46),  // "LosCommon::LosToolChain_Const..."
        QT_MOC_LITERAL(88, 42),  // "LosCommon::LosToolChain_Const..."
        QT_MOC_LITERAL(131, 4),  // "tool"
        QT_MOC_LITERAL(136, 16),  // "onBuildToolReady"
        QT_MOC_LITERAL(153, 7),  // "exePath"
        QT_MOC_LITERAL(161, 4)   // "args"
    },
    "LosCore::LosRunManager",
    "onToolChainReady",
    "",
    "LosCommon::LosToolChain_Constants::LosLanguage",
    "LosCommon::LosToolChain_Constants::LosTool",
    "tool",
    "onBuildToolReady",
    "exePath",
    "args"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_LosCore__LosRunManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   26,    2, 0x08,    1 /* Private */,
       6,    3,   33,    2, 0x08,    5 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4, QMetaType::QString,    2,    5,    2,
    QMetaType::Void, 0x80000000 | 4, QMetaType::QString, QMetaType::QStringList,    5,    7,    8,

       0        // eod
};

Q_CONSTINIT const QMetaObject LosCore::LosRunManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_LosCore__LosRunManager.offsetsAndSizes,
    qt_meta_data_LosCore__LosRunManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_LosCore__LosRunManager_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LosRunManager, std::true_type>,
        // method 'onToolChainReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<LosCommon::LosToolChain_Constants::LosLanguage, std::false_type>,
        QtPrivate::TypeAndForceComplete<LosCommon::LosToolChain_Constants::LosTool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onBuildToolReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<LosCommon::LosToolChain_Constants::LosTool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>
    >,
    nullptr
} };

void LosCore::LosRunManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LosRunManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onToolChainReady((*reinterpret_cast< std::add_pointer_t<LosCommon::LosToolChain_Constants::LosLanguage>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<LosCommon::LosToolChain_Constants::LosTool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 1: _t->onBuildToolReady((*reinterpret_cast< std::add_pointer_t<LosCommon::LosToolChain_Constants::LosTool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject *LosCore::LosRunManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LosCore::LosRunManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LosCore__LosRunManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LosCore::LosRunManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

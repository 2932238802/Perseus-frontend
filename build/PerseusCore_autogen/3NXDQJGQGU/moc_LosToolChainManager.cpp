/****************************************************************************
** Meta object code from reading C++ file 'LosToolChainManager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/core/LosToolChainManager/LosToolChainManager.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LosToolChainManager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_LosCore__LosToolChainManager_t {
    uint offsetsAndSizes[16];
    char stringdata0[29];
    char stringdata1[25];
    char stringdata2[1];
    char stringdata3[47];
    char stringdata4[5];
    char stringdata5[43];
    char stringdata6[5];
    char stringdata7[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_LosCore__LosToolChainManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_LosCore__LosToolChainManager_t qt_meta_stringdata_LosCore__LosToolChainManager = {
    {
        QT_MOC_LITERAL(0, 28),  // "LosCore::LosToolChainManager"
        QT_MOC_LITERAL(29, 24),  // "onCheckLanguageToolchain"
        QT_MOC_LITERAL(54, 0),  // ""
        QT_MOC_LITERAL(55, 46),  // "LosCommon::LosToolChain_Const..."
        QT_MOC_LITERAL(102, 4),  // "lang"
        QT_MOC_LITERAL(107, 42),  // "LosCommon::LosToolChain_Const..."
        QT_MOC_LITERAL(150, 4),  // "tool"
        QT_MOC_LITERAL(155, 17)   // "onCheckSingleTool"
    },
    "LosCore::LosToolChainManager",
    "onCheckLanguageToolchain",
    "",
    "LosCommon::LosToolChain_Constants::LosLanguage",
    "lang",
    "LosCommon::LosToolChain_Constants::LosTool",
    "tool",
    "onCheckSingleTool"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_LosCore__LosToolChainManager[] = {

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
       1,    2,   26,    2, 0x0a,    1 /* Public */,
       7,    1,   31,    2, 0x0a,    4 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

Q_CONSTINIT const QMetaObject LosCore::LosToolChainManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_LosCore__LosToolChainManager.offsetsAndSizes,
    qt_meta_data_LosCore__LosToolChainManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_LosCore__LosToolChainManager_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LosToolChainManager, std::true_type>,
        // method 'onCheckLanguageToolchain'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<LosCommon::LosToolChain_Constants::LosLanguage, std::false_type>,
        QtPrivate::TypeAndForceComplete<LosCommon::LosToolChain_Constants::LosTool, std::false_type>,
        // method 'onCheckSingleTool'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<LosCommon::LosToolChain_Constants::LosTool, std::false_type>
    >,
    nullptr
} };

void LosCore::LosToolChainManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LosToolChainManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onCheckLanguageToolchain((*reinterpret_cast< std::add_pointer_t<LosCommon::LosToolChain_Constants::LosLanguage>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<LosCommon::LosToolChain_Constants::LosTool>>(_a[2]))); break;
        case 1: _t->onCheckSingleTool((*reinterpret_cast< std::add_pointer_t<LosCommon::LosToolChain_Constants::LosTool>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *LosCore::LosToolChainManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LosCore::LosToolChainManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LosCore__LosToolChainManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LosCore::LosToolChainManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

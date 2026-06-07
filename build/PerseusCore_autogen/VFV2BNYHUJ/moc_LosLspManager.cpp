/****************************************************************************
** Meta object code from reading C++ file 'LosLspManager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/core/LosLsp/LosLspManager/LosLspManager.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LosLspManager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_LosCore__LosLspManager_t {
    uint offsetsAndSizes[16];
    char stringdata0[23];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[43];
    char stringdata4[14];
    char stringdata5[8];
    char stringdata6[8];
    char stringdata7[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_LosCore__LosLspManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_LosCore__LosLspManager_t qt_meta_stringdata_LosCore__LosLspManager = {
    {
        QT_MOC_LITERAL(0, 22),  // "LosCore::LosLspManager"
        QT_MOC_LITERAL(23, 10),  // "onLspReady"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 42),  // "LosCommon::LosToolChain_Const..."
        QT_MOC_LITERAL(78, 13),  // "onFileRenamed"
        QT_MOC_LITERAL(92, 7),  // "oldPath"
        QT_MOC_LITERAL(100, 7),  // "newPath"
        QT_MOC_LITERAL(108, 10)   // "onSemantic"
    },
    "LosCore::LosLspManager",
    "onLspReady",
    "",
    "LosCommon::LosToolChain_Constants::LosTool",
    "onFileRenamed",
    "oldPath",
    "newPath",
    "onSemantic"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_LosCore__LosLspManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   32,    2, 0x08,    1 /* Private */,
       4,    2,   39,    2, 0x08,    5 /* Private */,
       7,    1,   44,    2, 0x08,    8 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::QStringList,    2,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

Q_CONSTINIT const QMetaObject LosCore::LosLspManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_LosCore__LosLspManager.offsetsAndSizes,
    qt_meta_data_LosCore__LosLspManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_LosCore__LosLspManager_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LosLspManager, std::true_type>,
        // method 'onLspReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<LosCommon::LosToolChain_Constants::LosTool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        // method 'onFileRenamed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onSemantic'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void LosCore::LosLspManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LosLspManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onLspReady((*reinterpret_cast< std::add_pointer_t<LosCommon::LosToolChain_Constants::LosTool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[3]))); break;
        case 1: _t->onFileRenamed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->onSemantic((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *LosCore::LosLspManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LosCore::LosLspManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LosCore__LosLspManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LosCore::LosLspManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE

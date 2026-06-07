/****************************************************************************
** Meta object code from reading C++ file 'LosSettingsUi.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/view/LosSettingsUi/LosSettingsUi.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LosSettingsUi.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_LosView__LosSettingsUi_t {
    uint offsetsAndSizes[24];
    char stringdata0[23];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[10];
    char stringdata5[43];
    char stringdata6[5];
    char stringdata7[20];
    char stringdata8[6];
    char stringdata9[15];
    char stringdata10[13];
    char stringdata11[25];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_LosView__LosSettingsUi_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_LosView__LosSettingsUi_t qt_meta_stringdata_LosView__LosSettingsUi = {
    {
        QT_MOC_LITERAL(0, 22),  // "LosView::LosSettingsUi"
        QT_MOC_LITERAL(23, 14),  // "onSaveSettings"
        QT_MOC_LITERAL(38, 0),  // ""
        QT_MOC_LITERAL(39, 13),  // "onFindExePath"
        QT_MOC_LITERAL(53, 9),  // "file_path"
        QT_MOC_LITERAL(63, 42),  // "LosCommon::LosToolChain_Const..."
        QT_MOC_LITERAL(106, 4),  // "tool"
        QT_MOC_LITERAL(111, 19),  // "onThemeComboChanged"
        QT_MOC_LITERAL(131, 5),  // "index"
        QT_MOC_LITERAL(137, 14),  // "onFormatReload"
        QT_MOC_LITERAL(152, 12),  // "onFormatSave"
        QT_MOC_LITERAL(165, 24)   // "onCMakeInstallBtnClicked"
    },
    "LosView::LosSettingsUi",
    "onSaveSettings",
    "",
    "onFindExePath",
    "file_path",
    "LosCommon::LosToolChain_Constants::LosTool",
    "tool",
    "onThemeComboChanged",
    "index",
    "onFormatReload",
    "onFormatSave",
    "onCMakeInstallBtnClicked"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_LosView__LosSettingsUi[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x08,    1 /* Private */,
       3,    2,   51,    2, 0x08,    2 /* Private */,
       7,    1,   56,    2, 0x08,    5 /* Private */,
       9,    0,   59,    2, 0x08,    7 /* Private */,
      10,    0,   60,    2, 0x08,    8 /* Private */,
      11,    0,   61,    2, 0x0a,    9 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 5,    4,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject LosView::LosSettingsUi::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_LosView__LosSettingsUi.offsetsAndSizes,
    qt_meta_data_LosView__LosSettingsUi,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_LosView__LosSettingsUi_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LosSettingsUi, std::true_type>,
        // method 'onSaveSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFindExePath'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<LosCommon::LosToolChain_Constants::LosTool, std::false_type>,
        // method 'onThemeComboChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onFormatReload'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFormatSave'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCMakeInstallBtnClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void LosView::LosSettingsUi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LosSettingsUi *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onSaveSettings(); break;
        case 1: _t->onFindExePath((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<LosCommon::LosToolChain_Constants::LosTool>>(_a[2]))); break;
        case 2: _t->onThemeComboChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->onFormatReload(); break;
        case 4: _t->onFormatSave(); break;
        case 5: _t->onCMakeInstallBtnClicked(); break;
        default: ;
        }
    }
}

const QMetaObject *LosView::LosSettingsUi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LosView::LosSettingsUi::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LosView__LosSettingsUi.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int LosView::LosSettingsUi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

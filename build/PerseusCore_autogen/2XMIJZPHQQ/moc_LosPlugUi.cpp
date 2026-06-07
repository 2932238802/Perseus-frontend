/****************************************************************************
** Meta object code from reading C++ file 'LosPlugUi.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/view/LosPlugUi/LosPlugUi.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LosPlugUi.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_LosView__LosPlugUi_t {
    uint offsetsAndSizes[14];
    char stringdata0[19];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[47];
    char stringdata4[8];
    char stringdata5[20];
    char stringdata6[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_LosView__LosPlugUi_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_LosView__LosPlugUi_t qt_meta_stringdata_LosView__LosPlugUi = {
    {
        QT_MOC_LITERAL(0, 18),  // "LosView::LosPlugUi"
        QT_MOC_LITERAL(19, 13),  // "onPluginReply"
        QT_MOC_LITERAL(33, 0),  // ""
        QT_MOC_LITERAL(34, 46),  // "QList<LosCommon::LosNet_Const..."
        QT_MOC_LITERAL(81, 7),  // "plugins"
        QT_MOC_LITERAL(89, 19),  // "onPluginItemClicked"
        QT_MOC_LITERAL(109, 5)   // "index"
    },
    "LosView::LosPlugUi",
    "onPluginReply",
    "",
    "QList<LosCommon::LosNet_Constants::PluginInfo>",
    "plugins",
    "onPluginItemClicked",
    "index"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_LosView__LosPlugUi[] = {

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
       1,    1,   26,    2, 0x08,    1 /* Private */,
       5,    1,   29,    2, 0x08,    3 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

Q_CONSTINIT const QMetaObject LosView::LosPlugUi::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_LosView__LosPlugUi.offsetsAndSizes,
    qt_meta_data_LosView__LosPlugUi,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_LosView__LosPlugUi_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LosPlugUi, std::true_type>,
        // method 'onPluginReply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<LosCommon::LosNet_Constants::PluginInfo> &, std::false_type>,
        // method 'onPluginItemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void LosView::LosPlugUi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LosPlugUi *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onPluginReply((*reinterpret_cast< std::add_pointer_t<QList<LosCommon::LosNet_Constants::PluginInfo>>>(_a[1]))); break;
        case 1: _t->onPluginItemClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *LosView::LosPlugUi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LosView::LosPlugUi::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LosView__LosPlugUi.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LosView::LosPlugUi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

/****************************************************************************
** Meta object code from reading C++ file 'LosCommandUi.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/view/LosCommandUi/LosCommandUi.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LosCommandUi.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_LosView__LosCommandUi_t {
    uint offsetsAndSizes[26];
    char stringdata0[22];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[20];
    char stringdata5[13];
    char stringdata6[12];
    char stringdata7[12];
    char stringdata8[8];
    char stringdata9[8];
    char stringdata10[6];
    char stringdata11[14];
    char stringdata12[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_LosView__LosCommandUi_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_LosView__LosCommandUi_t qt_meta_stringdata_LosView__LosCommandUi = {
    {
        QT_MOC_LITERAL(0, 21),  // "LosView::LosCommandUi"
        QT_MOC_LITERAL(22, 19),  // "onSearchTextChanged"
        QT_MOC_LITERAL(42, 0),  // ""
        QT_MOC_LITERAL(43, 4),  // "text"
        QT_MOC_LITERAL(48, 19),  // "executeSelectedItem"
        QT_MOC_LITERAL(68, 12),  // "onPluginPath"
        QT_MOC_LITERAL(81, 11),  // "plugin_path"
        QT_MOC_LITERAL(93, 11),  // "eventFilter"
        QT_MOC_LITERAL(105, 7),  // "watched"
        QT_MOC_LITERAL(113, 7),  // "QEvent*"
        QT_MOC_LITERAL(121, 5),  // "event"
        QT_MOC_LITERAL(127, 13),  // "focusOutEvent"
        QT_MOC_LITERAL(141, 12)   // "QFocusEvent*"
    },
    "LosView::LosCommandUi",
    "onSearchTextChanged",
    "",
    "text",
    "executeSelectedItem",
    "onPluginPath",
    "plugin_path",
    "eventFilter",
    "watched",
    "QEvent*",
    "event",
    "focusOutEvent",
    "QFocusEvent*"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_LosView__LosCommandUi[] = {

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
       1,    1,   50,    2, 0x08,    1 /* Private */,
       4,    0,   53,    2, 0x08,    3 /* Private */,
       5,    1,   54,    2, 0x08,    4 /* Private */,
       7,    2,   57,    2, 0x09,    6 /* Protected */,
      11,    1,   62,    2, 0x09,    9 /* Protected */,
      10,    1,   65,    2, 0x09,   11 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Bool, QMetaType::QObjectStar, 0x80000000 | 9,    8,   10,
    QMetaType::Void, 0x80000000 | 12,   10,
    QMetaType::Bool, 0x80000000 | 9,   10,

       0        // eod
};

Q_CONSTINIT const QMetaObject LosView::LosCommandUi::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_LosView__LosCommandUi.offsetsAndSizes,
    qt_meta_data_LosView__LosCommandUi,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_LosView__LosCommandUi_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LosCommandUi, std::true_type>,
        // method 'onSearchTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'executeSelectedItem'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPluginPath'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'eventFilter'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QObject *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QEvent *, std::false_type>,
        // method 'focusOutEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QFocusEvent *, std::false_type>,
        // method 'event'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QEvent *, std::false_type>
    >,
    nullptr
} };

void LosView::LosCommandUi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LosCommandUi *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onSearchTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->executeSelectedItem(); break;
        case 2: _t->onPluginPath((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: { bool _r = _t->eventFilter((*reinterpret_cast< std::add_pointer_t<QObject*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QEvent*>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->focusOutEvent((*reinterpret_cast< std::add_pointer_t<QFocusEvent*>>(_a[1]))); break;
        case 5: { bool _r = _t->event((*reinterpret_cast< std::add_pointer_t<QEvent*>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *LosView::LosCommandUi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LosView::LosCommandUi::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LosView__LosCommandUi.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int LosView::LosCommandUi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

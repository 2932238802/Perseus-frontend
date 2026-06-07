/****************************************************************************
** Meta object code from reading C++ file 'LosMDPreview.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/view/LosPreview/LosMDPreview/LosMDPreview.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LosMDPreview.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_LosView__LosMDPreview_t {
    uint offsetsAndSizes[8];
    char stringdata0[22];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[4];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_LosView__LosMDPreview_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_LosView__LosMDPreview_t qt_meta_stringdata_LosView__LosMDPreview = {
    {
        QT_MOC_LITERAL(0, 21),  // "LosView::LosMDPreview"
        QT_MOC_LITERAL(22, 15),  // "onAnchorClicked"
        QT_MOC_LITERAL(38, 0),  // ""
        QT_MOC_LITERAL(39, 3)   // "url"
    },
    "LosView::LosMDPreview",
    "onAnchorClicked",
    "",
    "url"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_LosView__LosMDPreview[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x08,    1 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QUrl,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject LosView::LosMDPreview::staticMetaObject = { {
    QMetaObject::SuperData::link<LosPreview::staticMetaObject>(),
    qt_meta_stringdata_LosView__LosMDPreview.offsetsAndSizes,
    qt_meta_data_LosView__LosMDPreview,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_LosView__LosMDPreview_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LosMDPreview, std::true_type>,
        // method 'onAnchorClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QUrl &, std::false_type>
    >,
    nullptr
} };

void LosView::LosMDPreview::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LosMDPreview *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onAnchorClicked((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *LosView::LosMDPreview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LosView::LosMDPreview::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LosView__LosMDPreview.stringdata0))
        return static_cast<void*>(this);
    return LosPreview::qt_metacast(_clname);
}

int LosView::LosMDPreview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = LosPreview::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

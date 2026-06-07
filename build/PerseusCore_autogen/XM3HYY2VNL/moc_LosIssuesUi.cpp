/****************************************************************************
** Meta object code from reading C++ file 'LosIssuesUi.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/view/LosIssuesUi/LosIssuesUi.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LosIssuesUi.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_LosView__LosIssuesUi_t {
    uint offsetsAndSizes[18];
    char stringdata0[21];
    char stringdata1[21];
    char stringdata2[1];
    char stringdata3[4];
    char stringdata4[7];
    char stringdata5[14];
    char stringdata6[10];
    char stringdata7[50];
    char stringdata8[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_LosView__LosIssuesUi_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_LosView__LosIssuesUi_t qt_meta_stringdata_LosView__LosIssuesUi = {
    {
        QT_MOC_LITERAL(0, 20),  // "LosView::LosIssuesUi"
        QT_MOC_LITERAL(21, 20),  // "onTableDoubleClicked"
        QT_MOC_LITERAL(42, 0),  // ""
        QT_MOC_LITERAL(43, 3),  // "row"
        QT_MOC_LITERAL(47, 6),  // "colume"
        QT_MOC_LITERAL(54, 13),  // "onUpdateTable"
        QT_MOC_LITERAL(68, 9),  // "file_path"
        QT_MOC_LITERAL(78, 49),  // "QList<LosCommon::LosLsp_Const..."
        QT_MOC_LITERAL(128, 5)   // "diags"
    },
    "LosView::LosIssuesUi",
    "onTableDoubleClicked",
    "",
    "row",
    "colume",
    "onUpdateTable",
    "file_path",
    "QList<LosCommon::LosLsp_Constants::LosDiagnostic>",
    "diags"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_LosView__LosIssuesUi[] = {

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
       1,    2,   26,    2, 0x08,    1 /* Private */,
       5,    2,   31,    2, 0x08,    4 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 7,    6,    8,

       0        // eod
};

Q_CONSTINIT const QMetaObject LosView::LosIssuesUi::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_LosView__LosIssuesUi.offsetsAndSizes,
    qt_meta_data_LosView__LosIssuesUi,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_LosView__LosIssuesUi_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LosIssuesUi, std::true_type>,
        // method 'onTableDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onUpdateTable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<LosCommon::LosLsp_Constants::LosDiagnostic> &, std::false_type>
    >,
    nullptr
} };

void LosView::LosIssuesUi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LosIssuesUi *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onTableDoubleClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->onUpdateTable((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<LosCommon::LosLsp_Constants::LosDiagnostic>>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *LosView::LosIssuesUi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LosView::LosIssuesUi::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LosView__LosIssuesUi.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LosView::LosIssuesUi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

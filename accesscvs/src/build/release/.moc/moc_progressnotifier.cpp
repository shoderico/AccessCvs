/****************************************************************************
** Meta object code from reading C++ file 'progressnotifier.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../util/progressnotifier.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'progressnotifier.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ProgressNotifier_t {
    QByteArrayData data[14];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProgressNotifier_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProgressNotifier_t qt_meta_stringdata_ProgressNotifier = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ProgressNotifier"
QT_MOC_LITERAL(1, 17, 5), // "start"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 4), // "type"
QT_MOC_LITERAL(4, 29, 5), // "count"
QT_MOC_LITERAL(5, 35, 6), // "change"
QT_MOC_LITERAL(6, 42, 5), // "value"
QT_MOC_LITERAL(7, 48, 3), // "end"
QT_MOC_LITERAL(8, 52, 20), // "progressRangeChanged"
QT_MOC_LITERAL(9, 73, 7), // "minimum"
QT_MOC_LITERAL(10, 81, 7), // "maximum"
QT_MOC_LITERAL(11, 89, 20), // "progressValueChanged"
QT_MOC_LITERAL(12, 110, 13), // "progressValue"
QT_MOC_LITERAL(13, 124, 8) // "finished"

    },
    "ProgressNotifier\0start\0\0type\0count\0"
    "change\0value\0end\0progressRangeChanged\0"
    "minimum\0maximum\0progressValueChanged\0"
    "progressValue\0finished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProgressNotifier[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,
       5,    2,   49,    2, 0x06 /* Public */,
       7,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,   57,    2, 0x0a /* Public */,
      11,    1,   62,    2, 0x0a /* Public */,
      13,    0,   65,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    6,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,

       0        // eod
};

void ProgressNotifier::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ProgressNotifier *_t = static_cast<ProgressNotifier *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->start((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->change((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->end((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->progressRangeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->progressValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->finished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ProgressNotifier::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ProgressNotifier::start)) {
                *result = 0;
            }
        }
        {
            typedef void (ProgressNotifier::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ProgressNotifier::change)) {
                *result = 1;
            }
        }
        {
            typedef void (ProgressNotifier::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ProgressNotifier::end)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject ProgressNotifier::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ProgressNotifier.data,
      qt_meta_data_ProgressNotifier,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ProgressNotifier::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProgressNotifier::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ProgressNotifier.stringdata0))
        return static_cast<void*>(const_cast< ProgressNotifier*>(this));
    return QObject::qt_metacast(_clname);
}

int ProgressNotifier::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ProgressNotifier::start(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ProgressNotifier::change(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ProgressNotifier::end(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

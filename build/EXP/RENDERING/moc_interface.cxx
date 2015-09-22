/****************************************************************************
** Meta object code from reading C++ file 'interface.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../EXP/RENDERING/interface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Interface[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      18,   10,   10,   10, 0x08,
      32,   10,   10,   10, 0x08,
      49,   10,   10,   10, 0x08,
      64,   10,   10,   10, 0x08,
      78,   10,   10,   10, 0x08,
      92,   10,   10,   10, 0x08,
     105,   10,   10,   10, 0x08,
     123,   10,   10,   10, 0x08,
     139,   10,   10,   10, 0x08,
     162,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Interface[] = {
    "Interface\0\0open()\0active_play()\0"
    "active_advance()\0active_pause()\0"
    "active_stop()\0active_quit()\0active_rec()\0"
    "active_stop_rec()\0updateButtons()\0"
    "setGenotypeNumber(int)\0setFrameRate(int)\0"
};

void Interface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Interface *_t = static_cast<Interface *>(_o);
        switch (_id) {
        case 0: _t->open(); break;
        case 1: _t->active_play(); break;
        case 2: _t->active_advance(); break;
        case 3: _t->active_pause(); break;
        case 4: _t->active_stop(); break;
        case 5: _t->active_quit(); break;
        case 6: _t->active_rec(); break;
        case 7: _t->active_stop_rec(); break;
        case 8: _t->updateButtons(); break;
        case 9: _t->setGenotypeNumber((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setFrameRate((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Interface::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Interface::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Interface,
      qt_meta_data_Interface, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Interface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Interface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Interface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Interface))
        return static_cast<void*>(const_cast< Interface*>(this));
    return QWidget::qt_metacast(_clname);
}

int Interface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

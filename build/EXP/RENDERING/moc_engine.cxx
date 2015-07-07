/****************************************************************************
** Meta object code from reading C++ file 'engine.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../EXP/RENDERING/engine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'engine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Engine[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      22,    7,    7,    7, 0x05,
      33,    7,    7,    7, 0x05,
      44,    7,    7,    7, 0x05,
      53,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      63,    7,    7,    7, 0x0a,
      76,    7,    7,    7, 0x0a,
      86,    7,    7,    7, 0x0a,
      97,    7,    7,    7, 0x0a,
     104,    7,    7,    7, 0x0a,
     112,    7,    7,    7, 0x0a,
     119,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Engine[] = {
    "Engine\0\0initialized()\0finished()\0"
    "advanced()\0paused()\0stopped()\0"
    "initialize()\0advance()\0finalize()\0"
    "play()\0pause()\0stop()\0setDelay(uint)\0"
};

void Engine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Engine *_t = static_cast<Engine *>(_o);
        switch (_id) {
        case 0: _t->initialized(); break;
        case 1: _t->finished(); break;
        case 2: _t->advanced(); break;
        case 3: _t->paused(); break;
        case 4: _t->stopped(); break;
        case 5: _t->initialize(); break;
        case 6: _t->advance(); break;
        case 7: _t->finalize(); break;
        case 8: _t->play(); break;
        case 9: _t->pause(); break;
        case 10: _t->stop(); break;
        case 11: _t->setDelay((*reinterpret_cast< uint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Engine::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Engine::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Engine,
      qt_meta_data_Engine, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Engine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Engine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Engine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Engine))
        return static_cast<void*>(const_cast< Engine*>(this));
    return QObject::qt_metacast(_clname);
}

int Engine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Engine::initialized()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Engine::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Engine::advanced()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Engine::paused()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Engine::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE

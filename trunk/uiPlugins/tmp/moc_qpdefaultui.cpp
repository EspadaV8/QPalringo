/****************************************************************************
** Meta object code from reading C++ file 'qpdefaultui.h'
**
** Created: Wed Sep 9 22:23:21 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qpDefaultUi/qpdefaultui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qpdefaultui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QPDefaultUi[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_QPDefaultUi[] = {
    "QPDefaultUi\0"
};

const QMetaObject QPDefaultUi::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QPDefaultUi,
      qt_meta_data_QPDefaultUi, 0 }
};

const QMetaObject *QPDefaultUi::metaObject() const
{
    return &staticMetaObject;
}

void *QPDefaultUi::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPDefaultUi))
        return static_cast<void*>(const_cast< QPDefaultUi*>(this));
    if (!strcmp(_clname, "UiPluginInterface"))
        return static_cast< UiPluginInterface*>(const_cast< QPDefaultUi*>(this));
    if (!strcmp(_clname, "qpalringo.UiPluginInterface/0.1"))
        return static_cast< UiPluginInterface*>(const_cast< QPDefaultUi*>(this));
    return QObject::qt_metacast(_clname);
}

int QPDefaultUi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

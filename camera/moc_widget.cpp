/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Widget_t {
    QByteArrayData data[22];
    char stringdata0[490];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Widget_t qt_meta_stringdata_Widget = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Widget"
QT_MOC_LITERAL(1, 7, 32), // "on_pushButton_openserial_clicked"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 10), // "onSendData"
QT_MOC_LITERAL(4, 52, 29), // "on_pushButton_paizhao_clicked"
QT_MOC_LITERAL(5, 82, 34), // "on_pushButton_chakantupian_cl..."
QT_MOC_LITERAL(6, 117, 32), // "on_pushButton_controlled_clicked"
QT_MOC_LITERAL(7, 150, 34), // "on_pushButton_controlmotor_cl..."
QT_MOC_LITERAL(8, 185, 34), // "on_pushButton_motorforward_cl..."
QT_MOC_LITERAL(9, 220, 31), // "on_pushButton_motorback_clicked"
QT_MOC_LITERAL(10, 252, 36), // "on_horizontalSlider_led_value..."
QT_MOC_LITERAL(11, 289, 5), // "value"
QT_MOC_LITERAL(12, 295, 38), // "on_horizontalSlider_servo_val..."
QT_MOC_LITERAL(13, 334, 38), // "on_horizontalSlider_motor_val..."
QT_MOC_LITERAL(14, 373, 15), // "onMqttConnected"
QT_MOC_LITERAL(15, 389, 21), // "onMqttMessageReceived"
QT_MOC_LITERAL(16, 411, 7), // "message"
QT_MOC_LITERAL(17, 419, 14), // "QMqttTopicName"
QT_MOC_LITERAL(18, 434, 5), // "topic"
QT_MOC_LITERAL(19, 440, 18), // "onMqttStateChanged"
QT_MOC_LITERAL(20, 459, 24), // "QMqttClient::ClientState"
QT_MOC_LITERAL(21, 484, 5) // "state"

    },
    "Widget\0on_pushButton_openserial_clicked\0"
    "\0onSendData\0on_pushButton_paizhao_clicked\0"
    "on_pushButton_chakantupian_clicked\0"
    "on_pushButton_controlled_clicked\0"
    "on_pushButton_controlmotor_clicked\0"
    "on_pushButton_motorforward_clicked\0"
    "on_pushButton_motorback_clicked\0"
    "on_horizontalSlider_led_valueChanged\0"
    "value\0on_horizontalSlider_servo_valueChanged\0"
    "on_horizontalSlider_motor_valueChanged\0"
    "onMqttConnected\0onMqttMessageReceived\0"
    "message\0QMqttTopicName\0topic\0"
    "onMqttStateChanged\0QMqttClient::ClientState\0"
    "state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Widget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    1,   92,    2, 0x08 /* Private */,
      12,    1,   95,    2, 0x08 /* Private */,
      13,    1,   98,    2, 0x08 /* Private */,
      14,    0,  101,    2, 0x08 /* Private */,
      15,    2,  102,    2, 0x08 /* Private */,
      19,    1,  107,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 17,   16,   18,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Widget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_openserial_clicked(); break;
        case 1: _t->onSendData(); break;
        case 2: _t->on_pushButton_paizhao_clicked(); break;
        case 3: _t->on_pushButton_chakantupian_clicked(); break;
        case 4: _t->on_pushButton_controlled_clicked(); break;
        case 5: _t->on_pushButton_controlmotor_clicked(); break;
        case 6: _t->on_pushButton_motorforward_clicked(); break;
        case 7: _t->on_pushButton_motorback_clicked(); break;
        case 8: _t->on_horizontalSlider_led_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_horizontalSlider_servo_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_horizontalSlider_motor_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onMqttConnected(); break;
        case 12: _t->onMqttMessageReceived((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QMqttTopicName(*)>(_a[2]))); break;
        case 13: _t->onMqttStateChanged((*reinterpret_cast< QMqttClient::ClientState(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttTopicName >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttClient::ClientState >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Widget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Widget.data,
    qt_meta_data_Widget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Widget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

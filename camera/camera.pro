QT       += core gui widgets network serialport sql mqtt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mjpgstreamviewer.cpp \
    photobrowser.cpp \
    widget.cpp \
    keyboard.cpp \
    logindialog.cpp

HEADERS += \
    mjpgstreamviewer.h \
    photobrowser.h \
    widget.h \
    keyboard.h \
    logindialog.h

FORMS += \
    widget.ui

# 资源文件
RESOURCES += resources.qrc

# 部署规则
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

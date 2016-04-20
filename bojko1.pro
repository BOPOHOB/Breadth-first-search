TEMPLATE = app

QT += widgets core

SOURCES += main.cpp \
    kernel/brick.cpp \
    kernel/datum.cpp \
    gui.cpp \
    application.cpp

RESOURCES += \
    res.qrc

HEADERS += \
    kernel/brick.h \
    kernel/datum.h \
    gui.h \
    application.h


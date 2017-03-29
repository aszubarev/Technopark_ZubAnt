TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    vector.c \
    string_t.c \
    set_operator.c \
    counter.c \
    buffer.c

HEADERS += \
    vector.h \
    string_t.h \
    set_operator.h \
    counter.h \
    buffer.h

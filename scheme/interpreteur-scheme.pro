TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXX = gcc
QMAKE_CFLAGS = -x c -g -DDEBUG

HEADERS += include/basic.h \
           include/eval.h \
           include/mem.h \
           include/notify.h \
           include/number.h \
           include/object.h \
           include/print.h \
           include/read.h \
           include/boolean.h \
           include/symbols.h \
           include/forms.h \
    include/primitives.h

SOURCES += src/eval.c \
           src/mem.c \
           src/object.c \
           src/print.c \
           src/read.c \
           src/repl.c \
    src/symbols.c \
    src/forms.c \
    src/primitives.c

OTHER_FILES += .astylerc

INCLUDEPATH += include

LIBS += -lreadline

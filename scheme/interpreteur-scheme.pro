TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += include/basic.h \
           include/eval.h \
           include/mem.h \
           include/notify.h \
           include/number.h \
           include/object.h \
           include/print.h \
           include/read.h

SOURCES += src/eval.c \
           src/mem.c \
           src/object.c \
           src/print.c \
           src/read.c \
           src/repl.c

OTHER_FILES += .astylerc

INCLUDEPATH += include

LIBS += -lreadline

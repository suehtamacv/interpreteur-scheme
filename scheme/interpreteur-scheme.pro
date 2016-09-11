TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += eval.c \
           mem.c \
           object.c \
           print.c \
           read.c \
           repl.c

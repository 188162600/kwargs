TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    src/tools/algorithm.h \
    src/tools/debug.h \
    src/tools/functional.h \
    src/tools/global.h \
    src/tools/typetraits.h

SOURCES += \
    src/tools/algorithm.cpp \
    src/tools/debug.cpp \
    src/tools/functional.cpp \
    src/tools/global.cpp \
    src/tools/typetraits.cpp \
    main.cpp



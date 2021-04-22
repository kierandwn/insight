QT += sql

DATA_SRC_PATH = "$$PWD/src"
DATA_INCLUDE_PATH = "$$PWD/include"

INCLUDEPATH += \
    $$DATA_INCLUDE_PATH

SOURCES += \
    $$DATA_SRC_PATH/table.cpp

HEADERS += \
    $$DATA_INCLUDE_PATH/table.h \
    $$DATA_INCLUDE_PATH/channel.h

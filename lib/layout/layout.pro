LAYOUT_SRC_PATH = "$$PWD/src"
LAYOUT_INCLUDE_PATH = "$$PWD/include"

GRAPHIC_INCLUDE_PATH = "$$PWD/graphic/include"

FORMS += \
    $$PWD/ui/mainwindow.ui

SOURCES += \
    $$LAYOUT_SRC_PATH/grid.cpp \
    $$LAYOUT_SRC_PATH/mainwindow.cpp

INCLUDEPATH += \
    $$LAYOUT_INCLUDE_PATH
    $$GRAPHIC_INCLUDE_PATH

HEADERS += \
    $$LAYOUT_INCLUDE_PATH/grid.h \
    $$LAYOUT_INCLUDE_PATH/mainwindow.h \
    $$GRAPHIC_INCLUDE_PATH/insight_graphic_base.h

SCATTER_SRC_PATH = "$$PWD/src"
SCATTER_INCLUDE_PATH = "$$PWD/include"
SCATTER_UI_PATH = "$$PWD/ui"

GRAPHIC_INCLUDE_PATH = "../include"
GRAPHIC_SRC_PATH = "$$PWD/../src"

SOURCES += \
    $$SCATTER_SRC_PATH/scatter.cpp
    $$GRAPHIC_SRC_PATH/defaults.cpp

INCLUDEPATH += \
    $$SCATTER_INCLUDE_PATH
    $$GRAPHIC_INCLUDE_PATH

HEADERS += \
    $$SCATTER_INCLUDE_PATH/scatter.h
    $$GRAPHIC_INCLUDE_PATH/insight_graphic_base.h
    $$GRAPHIC_INCLUDE_PATH/linked_graphic.h

FORMS += \
    $${SCATTER_UI_PATH}/scatter.ui
    
QT += designer widgets uiplugin
CONFIG += plugin

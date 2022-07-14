SCATTER_SRC_PATH = "$$PWD/src"
SCATTER_INCLUDE_PATH = "$$PWD/include"
SCATTER_UI_PATH = "$$PWD/ui"

GRAPHIC_INCLUDE_PATH = "$$PWD/../include"
GRAPHIC_SRC_PATH = "$$PWD/../src"

SOURCES += \
    $$SCATTER_SRC_PATH/scatterdisplay.cpp \
    $$SCATTER_SRC_PATH/scattergroup.cpp \
    $$SCATTER_SRC_PATH/sc_mouse_events.cpp \
    $$SCATTER_SRC_PATH/crosshair.cpp \

INCLUDEPATH += \
    $$SCATTER_INCLUDE_PATH
    $$GRAPHIC_INCLUDE_PATH

HEADERS += \
    $$PWD/include/Crosshair.h \
    $$SCATTER_INCLUDE_PATH/scatterdisplay.h \
    $$SCATTER_INCLUDE_PATH/scattergroup.h \
    $$SCATTER_INCLUDE_PATH/crosshair.h \
    $$GRAPHIC_INCLUDE_PATH/ApplicationInterface.h \
    $$GRAPHIC_INCLUDE_PATH/linked_graphic.h

FORMS += \
    $${SCATTER_UI_PATH}/scatter.ui
    
QT += designer widgets uiplugin
CONFIG += plugin

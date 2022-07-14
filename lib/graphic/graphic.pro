GRAPHIC_INCLUDE_PATH = "$$PWD/include"
GRAPHIC_SRC_PATH = "$$PWD/src"

SOURCES += \
	$$GRAPHIC_SRC_PATH/defaults.cpp \
        $$GRAPHIC_SRC_PATH/vlabel.cpp \
        $$GRAPHIC_SRC_PATH/painting_tools.cpp \
	$$GRAPHIC_SRC_PATH/mouse_state.cpp

HEADERS += \
    $$GRAPHIC_INCLUDE_PATH/ApplicationInterface.h \
    $$GRAPHIC_INCLUDE_PATH/linked_graphic.h \
    $$GRAPHIC_INCLUDE_PATH/defaults.h \
    $$GRAPHIC_INCLUDE_PATH/vlabel.h \
    $$GRAPHIC_INCLUDE_PATH/mouse_state.h \
    $$GRAPHIC_INCLUDE_PATH/painting_tools.h

include(waveform/waveform.pri)
include(scatter/scatter.pri)

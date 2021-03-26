GRAPHIC_INCLUDE_PATH = "$$PWD/include"
GRAPHIC_SRC_PATH = "$$PWD/src"

SOURCES += \
	$$GRAPHIC_SRC_PATH/defaults.cpp

HEADERS += \
    $$GRAPHIC_INCLUDE_PATH/defaults.h \
    $$GRAPHIC_INCLUDE_PATH/insight_graphic_base.h \
    $$GRAPHIC_INCLUDE_PATH/linked_graphic.h

include(waveform/waveform.pri)
include(scatter/scatter.pri)

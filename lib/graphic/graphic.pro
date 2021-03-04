GRAPHIC_INCLUDE_PATH = "$$PWD/include"
GRAPHIC_SRC_PATH = "$$PWD/src"


INCLUDEPATH += \
    $$GRAPHIC_INCLUDE_PATH

HEADERS += \
    $$GRAPHIC_INCLUDE_PATH/insight_graphic_base.h

include(waveform/waveform.pri)

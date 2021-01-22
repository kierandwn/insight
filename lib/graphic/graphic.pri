# TEMPLATE = lib

GRAPHIC_INCLUDE_PATH = "$$PWD/include"

INCLUDEPATH += \
    $$GRAPHIC_INCLUDE_PATH

HEADERS += \
    $$GRAPHIC_INCLUDE_PATH/insight_graphic_base.h

include(waveform/waveform.pri)

WAVEFORM_SRC_PATH = "$$PWD/src"
WAVEFORM_INCLUDE_PATH = "$$PWD/include"
WAVEFORM_UI_PATH = "$$PWD/ui"

GRAPHIC_INCLUDE_PATH = "$$PWD/../include"
GRAPHIC_SRC_PATH = "$$PWD/../src"

SOURCES += \
    $$WAVEFORM_SRC_PATH/waveformdisplay.cpp \
    $$WAVEFORM_SRC_PATH/waveformgroup.cpp \
    $$WAVEFORM_SRC_PATH/wf_mouse_events.cpp

INCLUDEPATH += \
    $$WAVEFORM_INCLUDE_PATH \
    $$GRAPHIC_INCLUDE_PATH

HEADERS += \
    $$WAVEFORM_INCLUDE_PATH/waveformdisplay.h \
    $$WAVEFORM_INCLUDE_PATH/waveformgroup.h \
    $$GRAPHIC_INCLUDE_PATH/ApplicationInterface.h \
    $$GRAPHIC_INCLUDE_PATH/linked_graphic.h

FORMS += \
    $${WAVEFORM_UI_PATH}/waveform.ui
    
QT += designer widgets uiplugin
CONFIG += plugin

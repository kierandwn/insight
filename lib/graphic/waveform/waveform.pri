WAVEFORM_SRC_PATH = "$$PWD/src"
WAVEFORM_INCLUDE_PATH = "$$PWD/include"
WAVEFORM_UI_PATH = "$$PWD/ui"

SOURCES += \
    $$WAVEFORM_SRC_PATH/waveformdisplay.cpp
    ../src/base.cpp

INCLUDEPATH += \
    $$WAVEFORM_INCLUDE_PATH

HEADERS += \
    $$WAVEFORM_INCLUDE_PATH/waveformdisplay.h

FORMS += \
    $${WAVEFORM_UI_PATH}/waveform.ui
    
QT += designer widgets uiplugin
CONFIG += plugin

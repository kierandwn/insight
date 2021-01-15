SOURCES += \
    lib/graphic/waveform/waveformdisplay.cpp

HEADERS += \
    lib/graphic/include/insight_graphic_base.h \
    lib/graphic/waveform/include/waveformdisplay.h

FORMS += \
    lib/graphic/waveform/ui/waveform.ui

    
QT += designer widgets uiplugin
CONFIG += plugin
#TEMPLATE = lib

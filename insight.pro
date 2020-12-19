QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include ( C:/qwt-6.1.5/features/qwt.prf )

include ( lib/config/config.pro )
# include ( lib/data/data.pro )
include ( lib/graphic/graphic.pro )
include ( lib/layout/layout.pro )

SOURCES += \
    main.cpp

FORMS += \
    ui/mainwindow.ui

DISTFILES += \
    config/layout.config

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

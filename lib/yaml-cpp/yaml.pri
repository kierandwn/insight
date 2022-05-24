#TEMPLATE = lib
#CONFIG += static

INCLUDEPATH += \
    $$PWD/include \
    $$PWD/src

SOURCES += \
    $$PWD/src/binary.cpp \
    $$PWD/src/convert.cpp \
    $$PWD/src/directives.cpp \
    $$PWD/src/emit.cpp \
    $$PWD/src/emitfromevents.cpp \
    $$PWD/src/emitter.cpp \
    $$PWD/src/emitterstate.cpp \
    $$PWD/src/emitterutils.cpp \
    $$PWD/src/exceptions.cpp \
    $$PWD/src/exp.cpp \
    $$PWD/src/memory.cpp \
    $$PWD/src/node.cpp \
    $$PWD/src/node_data.cpp \
    $$PWD/src/nodebuilder.cpp \
    $$PWD/src/nodeevents.cpp \
    $$PWD/src/null.cpp \
    $$PWD/src/ostream_wrapper.cpp \
    $$PWD/src/parse.cpp \
    $$PWD/src/parser.cpp \
    $$PWD/src/regex_yaml.cpp \
    $$PWD/src/scanner.cpp \
    $$PWD/src/scanscalar.cpp \
    $$PWD/src/scantag.cpp \
    $$PWD/src/scantoken.cpp \
    $$PWD/src/simplekey.cpp \
    $$PWD/src/singledocparser.cpp \
    $$PWD/src/stream.cpp \
    $$PWD/src/tag.cpp \
    $$PWD/src/contrib/graphbuilder.cpp \
    $$PWD/src/contrib/graphbuilderadapter.cpp

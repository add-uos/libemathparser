TEMPLATE = lib

TARGET = eparserqt

HEADERS += \
    $$PWD/easy_parser_qt.h

SOURCES += \
    $$PWD/easy_parser_qt.cpp

target.path = /usr/lib/
files.files += $$PWD/easy_parser_qt.h
files.path = /usr/inclue/libemathparser/

INSTALLS += target files
TEMPLATE = lib

TARGET = eparserstd

HEADERS += \
    $$PWD/easy_parser_std.h

SOURCES += \
    $$PWD/easy_parser_std.cpp

target.path = /usr/lib/
files.files += $$PWD/easy_parser_std.h
files.path = /usr/inclue/libemathparser/

INSTALLS += target files
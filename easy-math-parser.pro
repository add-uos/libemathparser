QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(common/common.pri)
include(parser_qt/parser_qt.pri)
include(parser_std/parser_std.pri)
include(muparser_engine/muparser_engine.pri)
include(gui_test/gui_test.pri)
SOURCES += \
    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

copy_file.input = $$PWD/parse
copy_file.output = $$OUT_PWD/parse
copy_file.CONFIG = verbatim
QMAKE_SUBSTITUTES += copy_file


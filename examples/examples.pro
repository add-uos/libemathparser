QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

include(common/common.pri)

INCLUDEPATH += ../

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

copy_file.input = $$PWD/parse
copy_file.output = $$OUT_PWD/parse
copy_file.CONFIG = verbatim
QMAKE_SUBSTITUTES += copy_file

FORMS += \
    $$PWD/MainWindow.ui

HEADERS += \
    $$PWD/MainWindow.h

SOURCES += \
    $$PWD/MainWindow.cpp \
    main.cpp

LIBS += -leparserqt -leparserstd -lmuparser

unix:!macx: LIBS += -L$$OUT_PWD/../parser_qt/ -leparserqt
unix:!macx: LIBS += -L$$OUT_PWD/../parser_std/ -leparserstd
unix:!macx: LIBS += -L$$OUT_PWD/../muparser_engine/ -lmuparser

target.path = /usr/bin/

INSTALLS += target
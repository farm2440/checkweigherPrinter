#-------------------------------------------------
#
# Project created by QtCreator 2014-06-10T09:38:15
#
#-------------------------------------------------

QT       += core gui xml serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChkWrPrint
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dlgItemSelect.cpp \
    dlgSelectLotNumber.cpp \
    dlgEditItems.cpp \
    dlgCommSettings.cpp \
    iniparser.cpp \
    dictionary.cpp \
    scale.cpp

HEADERS  += mainwindow.h \
    dlgItemSelect.h \
    dlgSelectLotNumber.h \
    dlgEditItems.h \
    dlgCommSettings.h \
    settings.h \
    iniparser.h \
    dictionary.h \
    scale.h

FORMS    += mainwindow.ui \
    dlgitemselect.ui \
    dlgSelectLotNumber.ui \
    dlgEditItems.ui \
    dlgCommSettings.ui

OTHER_FILES += \
    README.txt

RESOURCES += \
    res.qrc

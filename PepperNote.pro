#-------------------------------------------------
#
# Project created by QtCreator 2012-10-27T20:13:11
#
#-------------------------------------------------

QT       += core gui
QT       += webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PepperNote
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
        notewebview.cpp

HEADERS  += mainwindow.h \
        notewebview.h

FORMS    += mainwindow.ui

RESOURCES += \
    notes.qrc

OTHER_FILES += \
    res/editor/html/new_note.html \
    res/editor/css/note.css \
    res/editor/javascripts/note.js

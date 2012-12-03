#-------------------------------------------------
#
# Project created by QtCreator 2012-10-27T20:13:11
#
#-------------------------------------------------

QT       += core gui
QT       += webkit
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PepperNote
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
        notewebview.cpp \
        javascriptapi.cpp \
        notebook.cpp \
        notebookpage.cpp \
        notebookformat.cpp \
        notebookmanager.cpp \
        notebookformatmanifest.cpp \
        filenotebookformat.cpp \
        qt-json/json.cpp

HEADERS  += mainwindow.h \
        utility.h \
        notewebview.h \
        javascriptapi.h \
        notebook.h \
        notebookpage.h \
        notebookformat.h \
        notebookmanager.h \
        notebookformatmanifest.h \
        filenotebookformat.h \
        qt-json/json.h

FORMS    += mainwindow.ui

RESOURCES += \
    notes.qrc

OTHER_FILES += \
    res/editor/html/note.html \
    res/editor/html/new_note.html \
    res/editor/css/note.css \
    res/editor/javascripts/note.js \
    res/editor/javascripts/utility.js \
    res/editor/javascripts/prototype.js

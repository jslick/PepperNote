#-------------------------------------------------
#
# Project created by QtCreator 2012-10-27T20:13:11
#
#-------------------------------------------------

QT       += core gui
QT       += webkit
win32:QMAKE_CXXFLAGS += -std=c++0x
else:QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PepperNote
TEMPLATE = app

INCLUDEPATH += ../contrib/qt-json/

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
        notebooktree.cpp \
        treenotebookitem.cpp \
        treenotebookpageitem.cpp \
        lambdaguard.cpp \
        ../contrib/qt-json/json.cpp

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
        notebooktree.h \
        treenotebookitem.h \
        treenotebookpageitem.h \
        notebookexception.h \
        lambdaguard.h \
        ../contrib/qt-json/json.h

FORMS    += mainwindow.ui

RESOURCES += \
    notes.qrc

OTHER_FILES += \
    res/editor/html/page.html \
    res/editor/html/new_page.html \
    res/editor/css/page.css \
    res/editor/javascripts/page.js \
    res/editor/javascripts/utility.js \
    res/editor/javascripts/prototype.js

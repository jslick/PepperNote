/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

const QString APP_ORGNAME = "PepperNote";
const QString APP_APPNAME = "PepperNote";

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(APP_ORGNAME);
    QCoreApplication::setApplicationName(APP_APPNAME);
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}

#include <QApplication>
#include "mainwindow.h"

const QString APP_ORGNAME = "PepperNote";
const QString APP_APPNAME = "PepperNote";

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(APP_ORGNAME);
    QCoreApplication::setApplicationName(APP_APPNAME);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}

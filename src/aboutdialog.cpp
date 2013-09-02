/*
 * Copyright (c) 2012 PepperNote developers
 *
 * See the file license.txt for copying permission.
 */

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

extern const QString APP_APPNAME;

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QString about;
    about += QString("<h1>%1</h1>").arg(APP_APPNAME);
    about += QString("<p>%1</p>").arg(tr("Copyright 2012-2013 %1").arg("Jason Eslick"));
    about += QString("<p>%1:<a href=\"http://opensource.org/licenses/MIT\">http://opensource.org/licenses/MIT</a></p>")
            .arg(tr("This software is licensed under the MIT license"));
    about += QString("<p>%1</p>").arg(tr("Built with Qt %1").arg(QT_VERSION_STR));
    about += QString("<p>%1</p>").arg(tr("Qt runtime version is %1").arg(qVersion()));
#if defined(__DATE__) && defined(__TIME__)
    about += QString("<p>%1</p>").arg(tr("Built on %1 at %2").arg(__DATE__, __TIME__));
#endif

    this->ui->lblAboutText->setText(about);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

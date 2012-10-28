#include "notewebview.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QResource>

#include <QDir>
#include <QDebug>
void debugResources()
{
    QDir resDir(":/editor/css");
    QFileInfoList listing = resDir.entryInfoList();
    for (int i = 0; i < listing.size(); i++)
    {
        QFileInfo info = listing.at(i);
        qDebug() << info.fileName() << ' ';
    }
}

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), webView(new NoteWebView())
{
    this->ui->setupUi(this);
    this->resize(800, 640);

    this->ui->centralWidget->layout()->addWidget(this->webView);
    this->setNewNote();

    this->webView->focusWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <QWebFrame>
void MainWindow::setNewNote()
{
    QFile noteFile(":/editor/html/new_note.html");
    noteFile.open(QIODevice::ReadOnly);
    QString noteHtml = QString::fromUtf8(noteFile.readAll().constData());
    noteFile.close();

    this->webView->setHtml(noteHtml, QUrl("qrc:/editor/"));
    qDebug() << this->webView->page()->mainFrame()->baseUrl();
}

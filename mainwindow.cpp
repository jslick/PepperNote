#include "notewebview.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>

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

void MainWindow::setNewNote()
{
    QFile noteFile(":/editor/html/new_note.html");
    noteFile.open(QIODevice::ReadOnly);
    QString noteHtml = QString::fromUtf8(noteFile.readAll().constData());
    noteFile.close();

    this->webView->setHtml(noteHtml, QUrl("qrc:/editor/"));
}

#include "notebookpage.h"
#include "notewebview.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    NotebookPage* newPage = new NotebookPage;
    this->webView->setPage(newPage);
}

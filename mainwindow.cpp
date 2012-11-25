#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utility.h"
#include "notebookmanager.h"
#include "notebook.h"
#include "notebookpage.h"
#include "filenotebookformat.h"
#include "notewebview.h"

#include <QStringBuilder>
#include <QDir>
#include <QSettings>
#include <QDesktopServices>

QDir MainWindow::getNotebooksDirectory()
{
    QDir docsDir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    QDir notebooksDir = QString(docsDir.absolutePath() % '/' % QCoreApplication::applicationName() % " Notebooks");
    return notebooksDir;
}

QString MainWindow::getDefaultNotebookFilename()
{
    QDir notebooksDir = getNotebooksDirectory();
    QString defaultNotebookFilename = notebooksDir.absolutePath() % "/Personal";
    return defaultNotebookFilename;
}

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), webView(new NoteWebView())
{
    this->ui->setupUi(this);
    this->resize(800, 640);
    // TODO:  save & restore size
    // TODO:  save & restore state

    this->ui->centralWidget->layout()->addWidget(this->webView);

    this->initToolbar();

    connect(&NotebookManager::instance, SIGNAL(notebookLoaded(Notebook*)),
            this, SLOT(showLoadedNotebook(Notebook*))
            );

    this->loadNotebooks();
}

MainWindow::~MainWindow()
{
    while (this->loadedNotebooks.isEmpty() == false)
        delete this->loadedNotebooks.takeLast();

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    this->webView->closing();

    QMainWindow::closeEvent(event);
}

void MainWindow::initToolbar()
{
    this->ui->mainToolBar->addAction(this->webView->pageAction(QWebPage::ToggleBold));
    this->ui->mainToolBar->addAction(this->webView->pageAction(QWebPage::ToggleItalic));
    this->ui->mainToolBar->addAction(this->webView->pageAction(QWebPage::ToggleUnderline));
    this->ui->mainToolBar->addSeparator();
    this->ui->mainToolBar->addAction(this->webView->pageAction(QWebPage::InsertOrderedList));
    this->ui->mainToolBar->addAction(this->webView->pageAction(QWebPage::InsertUnorderedList));
}

void MainWindow::loadNotebooks()
{
    // Get the filename of the last-used notebook
    QSettings settings;
    QString notebookFilename;
    if (settings.contains("lastuse/notebook_filename"))
    {
        notebookFilename = settings.value("lastuse/notebook_filename").toString();
        if (!QFile::exists(notebookFilename))
        {
            showMessage(tr("Could not find notebook with filename:  ") % notebookFilename);

            notebookFilename = getDefaultNotebookFilename();
        }
    }
    else
    {
        notebookFilename = getDefaultNotebookFilename();
    }

    // Get filenames of all notebooks
    QDir notebooksDir = getNotebooksDirectory();
    // TODO:  formats should register extensions, which would be filtered here.
    QFileInfoList notebookInfos = notebooksDir.entryInfoList(/*filters*/);
    int numNotebooks = 0;

    QStringList filenames;
    for (int i = 0; i < notebookInfos.size(); i++)
    {
        // Load the last-used notebook first; queue the rest
        QFileInfo fileInfo = notebookInfos.at(i);
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if (fileInfo.isDir() == false)
            continue;
        if (fileInfo.fileName().contains("noload"))
            continue;
        numNotebooks++;

        if (fileInfo.fileName() == notebookFilename)
            NotebookManager::instance.loadNotebook(fileInfo.absoluteFilePath());
        else
            filenames << fileInfo.absoluteFilePath();
    }
    // Load the queued notebooks
    for (int i = 0; i < filenames.size(); i++)
    {
        NotebookManager::instance.loadNotebook(filenames.at(i));
        // Slot showLoadedNotebook
    }

    if (numNotebooks == 0)
    {
        // No notebooks found!  Create the default one.
        NotebookFormat* fileFormat = new FileNotebookFormat(notebookFilename);
        NotebookManager::instance.createNotebook(fileFormat);
        // Slot showLoadedNotebook
    }
}

void MainWindow::showLoadedNotebook(Notebook* notebook)
{
    CHECK_POINTER_GUI(notebook, tr("Could not load notebook"));

    this->loadedNotebooks.append(notebook);

    NotebookPage* firstPage = notebook->getFirstPage();
    CHECK_POINTER_GUI(firstPage, tr("Could not load first page of notebook"));

    this->webView->setPage(*firstPage);
}

#include "notebookmanager.h"
#include "notebook.h"
#include "notebookpage.h"
#include "notewebview.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringBuilder>
#include <QDir>
#include <QSettings>
#include <QDesktopServices>
#include <QMessageBox>

QDir MainWindow::getNotebooksDirectory()
{
    QDir docsDir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    QDir notebooksDir = docsDir.relativeFilePath(QCoreApplication::applicationName() % " Notebooks");
    return notebooksDir;
}

QString MainWindow::getDefaultNotebookFilename()
{
    QDir notebooksDir = getNotebooksDirectory();
    QString defaultNotebookFilename = notebooksDir.relativeFilePath("Personal." % Notebook::FILE_EXTENSION);
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

    connect(&NotebookManager::instance, SIGNAL(notebookLoaded(Notebook*)),
            this, SLOT(showLoadedNotebook(Notebook*))
            );

    this->loadNotebooks();
}

MainWindow::~MainWindow()
{
    delete ui;
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
            QMessageBox msgBox;
            msgBox.setText("Could not find notebook with filename:  " % notebookFilename);
            msgBox.exec();

            notebookFilename = getDefaultNotebookFilename();
        }
    }
    else
    {
        notebookFilename = getDefaultNotebookFilename();
    }

    // Get filenames of all notebooks
    QDir notebooksDir = getNotebooksDirectory();
    QStringList filters;
    filters << "*." % Notebook::FILE_EXTENSION;
    notebooksDir.setNameFilters(filters);
    QFileInfoList notebookInfos = notebooksDir.entryInfoList(filters);

    QStringList filenames;
    for (int i = 0; i < notebookInfos.size(); i++)
    {
        // Load the last-used notebook first; queue the rest
        QFileInfo fileInfo = notebookInfos.at(i);
        if (fileInfo.fileName() == notebookFilename)
            NotebookManager::instance.loadNotebook(fileInfo.absoluteFilePath());
        else
            filenames << fileInfo.absoluteFilePath();
    }
    // Load the queued notebooks
    for (int i = 0; i < filenames.size(); i++)
    {
        NotebookManager::instance.loadNotebook(filenames.at(i));
    }

    if (notebookInfos.isEmpty())
    {
        // No notebooks found!  Create the default one.
        NotebookManager::instance.createNotebook();
    }
}

void MainWindow::showLoadedNotebook(Notebook* notebook)
{
    // TODO:  validate param

    NotebookPage* firstPage = notebook->getFirstPage();
    // TODO:  validate
    this->webView->setPage(firstPage);
    this->webView->focusWidget();
}

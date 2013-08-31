/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utility.h"
#include "notebookmanager.h"
#include "notebook.h"
#include "notebookpage.h"
#include "filenotebookformat.h"
#include "javascriptapi.h"
#include "notebooktree.h"
#include "treenotebookpageitem.h"
#include "notewebview.h"

#include <QtCore>
#include <QStringBuilder>
#include <QDir>
#include <QSettings>
#if QT_VERSION < 0x050000
#  include <QDesktopServices>
#endif
#include <QDockWidget>
#include <QInputDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QDoubleValidator>

QDir MainWindow::getNotebooksDirectory()
{
#if QT_VERSION >= 0x050000
    QDir docsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#else
    QDir docsDir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
#endif
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
    ui(new Ui::MainWindow), trayIcon(new QSystemTrayIcon(this)), jsApi(new JavascriptApi),
    fileMenu(0), webView(new NoteWebView(*this->jsApi)),
    notebookTree(new NotebookTree), notebookTreeDock(new QDockWidget),
    fontbox(0), fontsizebox(0)
{
    this->ui->setupUi(this);

    this->setWindowIcon(QIcon(":/app/lame.png"));
    this->trayIcon->setIcon(this->windowIcon());
    this->trayIcon->show();
    connect(this->trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(handleTray(QSystemTrayIcon::ActivationReason))
            );

    this->resize(800, 640);

    QSettings settings;
    this->restoreGeometry(settings.value("window/geometry").toByteArray());
    this->restoreState(settings.value("window/state").toByteArray());

    // When the page title changes in the editor, update the NotebookPage name
    connect(this->jsApi, SIGNAL(titleChanged(QString)),
            SLOT(setCurrentPageName(QString))
            );

    this->ui->centralWidget->layout()->addWidget(this->webView);

    this->initMenubar();
    this->initNotebookTree();
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

    if (this->jsApi)
        delete this->jsApi;
}

void MainWindow::bringToFront()
{
    this->show();
    this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    this->raise();
    this->activateWindow();
    this->setFocus();
}

void MainWindow::changeEvent(QEvent* event)
{
    QMainWindow::changeEvent(event);

    // Hide on minimize (if pref set)
    if (event->type() == QEvent::WindowStateChange)
    {
        if (this->isMinimized())
        {
            QSettings settings;
            if (settings.value("window/hide-on-minimize", false).toBool())
                this->hide();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    this->webView->closing();

    QSettings settings;
    settings.setValue("window/state", this->saveState());
    this->showNormal();
    settings.setValue("window/geometry", this->saveGeometry());

    // Hide tray icon so that app.exec() call returns
    if (this->trayIcon)
        this->trayIcon->hide();

    QMainWindow::closeEvent(event);
}

void MainWindow::initMenubar()
{
    this->fileMenu = menuBar()->addMenu(tr("&File"));

    QAction* newSectionAction = new QAction(tr("New Section..."), this);
    connect(newSectionAction, SIGNAL(triggered()),
            SLOT(createNewSection())
            );
    fileMenu->addAction(newSectionAction);

    QAction* newPageAction = new QAction(tr("New Page"), this);
    newPageAction->setShortcut(QKeySequence::New);
    connect(newPageAction, SIGNAL(triggered()),
            SLOT(createNewPage())
            );
    fileMenu->addAction(newPageAction);

    fileMenu->addSeparator();
    QAction* exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()),
            SLOT(close())
            );
    fileMenu->addAction(exitAction);
}

void MainWindow::initToolbar()
{
#if ENABLE_TOOLBAR_FONT_CONTROLS
    // It works, but it's kinda nasty.  The font family seems to work pretty
    // well.  But, the font size doesn't yet create an undo stack item since it
    // had to be implemented through JavaScript.

    this->fontbox = new QFontComboBox;
    this->ui->mainToolBar->addWidget(fontbox);

    this->fontsizebox = new QComboBox;
    this->fontsizebox->insertItems(0, QString("6,7,8,9,10,11,12,14,16,18,20,22,24,26,28,30,32,36,40,46,64").split(','));
    this->fontsizebox->setEditable(true);
    QDoubleValidator* fontSizeValidator = new QDoubleValidator;
    fontSizeValidator->setRange(1, 64);
    fontSizeValidator->setDecimals(1);
    this->fontsizebox->setValidator(fontSizeValidator);
    this->fontsizebox->setEditText("10");
    this->ui->mainToolBar->addWidget(this->fontsizebox);

    connect(this->jsApi, SIGNAL(selectionChanged(QString,double)),
            SLOT(updateFontControls(QString,double))
            );

    this->connectFontControls();
#endif // ENABLE_TOOLBAR_FONT_CONTROLS

    const QWebPage::WebAction ACTIONS[] =
    {
        QWebPage::ToggleBold, QWebPage::ToggleItalic, QWebPage::ToggleUnderline,
        QWebPage::ToggleStrikethrough,
        QWebPage::NoWebAction, /* separator */
        QWebPage::InsertOrderedList, QWebPage::InsertUnorderedList,
        QWebPage::Outdent, QWebPage::Indent,
        QWebPage::RemoveFormat
    };
    const QString ACTION_STD_ICONS[] =
    {
        "format-text-bold", "format-text-italic", "format-text-underline",
        "format-text-strikethrough",
        "",
        "format-list-ordered", "format-list-unordered",
        "format-indent-less", "format-indent-more",
        "edit-clear-list"
    };

    for (size_t i = 0; i < sizeof(ACTIONS) / sizeof(QWebPage::WebAction); i++)
    {
        QWebPage::WebAction webAction = ACTIONS[i];
        if (webAction == QWebPage::NoWebAction)
        {
            this->ui->mainToolBar->addSeparator();
            continue;
        }

        QAction* action = this->webView->pageAction(webAction);
        if (action)
        {
            const QString& iconName = ACTION_STD_ICONS[i];
            if (iconName.isEmpty() == false)
                action->setIcon(QIcon::fromTheme(iconName));

            this->ui->mainToolBar->addAction(action);
        }
    }
}

void MainWindow::initNotebookTree()
{
    this->notebookTreeDock->setObjectName("Notebook Tree");
    this->notebookTreeDock->setWidget(this->notebookTree);
    this->notebookTreeDock->setFeatures(this->notebookTreeDock->features() & ~QDockWidget::DockWidgetFloatable);
    this->addDockWidget(Qt::LeftDockWidgetArea, this->notebookTreeDock);
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

    // Show notebook in tree widget
    if (this->notebookTree)
    {
        this->notebookTree->addNotebook(*notebook);
        // When the page changes, update the tree
        connect(this->webView, SIGNAL(pageChanged(Notebook*,NotebookPage*)),
                this->notebookTree, SLOT(selectPage(Notebook*,NotebookPage*))
                );
        // When tree item changes, show the new page
        connect(this->notebookTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                SLOT(switchPage(QTreeWidgetItem*,QTreeWidgetItem*))
                );
    }

    // Show first notebook page in the notebook
    NotebookPage* firstPage = notebook->getFirstPage();
    CHECK_POINTER_GUI(firstPage, tr("Could not load first page of notebook"));

    this->webView->setPage(*notebook, *firstPage);
}

void MainWindow::createNewPage()
{
    // Get the section that the current page is in.  The new page will initially
    // belong to the same section as the current open note.
    Notebook* selectedPageNotebook = 0;
    NotebookPage* selectedPage = 0;
    this->webView->getCurrentPage(selectedPageNotebook, selectedPage);
    Q_ASSERT(selectedPageNotebook);
    Q_ASSERT(selectedPage);
    QString currentSection = selectedPageNotebook->getPageSection(*selectedPage);

    QString newPageSection = currentSection.isEmpty() ? "General" : currentSection;

    NotebookPage* newPage = new NotebookPage("", tr("New Note"));
    this->loadedNotebooks[0]->addPage(newPageSection, newPage);
    this->notebookTree->addPageItem(*this->loadedNotebooks[0], newPageSection, *newPage);
    this->webView->setPage(*this->loadedNotebooks[0], *newPage);
}

void MainWindow::createNewSection()
{
    // Get the section that the current page is in.  The new page will initially
    // belong to the same section as the current open note.
    Notebook* selectedPageNotebook = 0;
    NotebookPage* selectedPage = 0;
    this->webView->getCurrentPage(selectedPageNotebook, selectedPage);
    Q_ASSERT(selectedPageNotebook);
    Q_ASSERT(selectedPage);

    QString newPageSection;
    do
    {
        newPageSection = QInputDialog::getText(
                    this, tr("New Section"),
                    tr("Name of new section:")
                    );

        if (newPageSection.isEmpty())
            return;

        bool alreadyExists = selectedPageNotebook->getSectionNames().contains(newPageSection);
        if (alreadyExists)
            showMessage(tr("A section of that name already exists"));
        else
            break;
    } while (true);

    NotebookPage* newPage = new NotebookPage("", tr("New Note"));

    this->loadedNotebooks[0]->addPage(newPageSection, newPage);
    this->notebookTree->addPageItem(*this->loadedNotebooks[0], newPageSection, *newPage);
    this->webView->setPage(*this->loadedNotebooks[0], *newPage);
}

void MainWindow::switchPage(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    Q_ASSERT(current);
    Q_UNUSED(previous);

    TreeNotebookPageItem* currentItem = dynamic_cast<TreeNotebookPageItem*>( current );
    if (currentItem)
    {
        Notebook& notebook = currentItem->getNotebook();
        NotebookPage& page = currentItem->getNotebookPage();

        this->webView->setPage(notebook, page);
    }
}

void MainWindow::setCurrentPageName(QString pageTitle)
{
    NotebookPage* page = this->webView->getCurrentPage();
    if (!page)
    {
        showMessage(tr("Cannot set current page title"));   // Hope user never never sees this
        return;
    }

    pageTitle = pageTitle.trimmed();

    if (pageTitle.isEmpty() == false)
        page->setName(pageTitle);
}

void MainWindow::connectFontControls()
{
    if (!this->webView)
    {
        qDebug() << "No webview";
        return;
    }

    if (this->fontbox)
    {
        connect(this->fontbox, SIGNAL(currentFontChanged(QFont)),
                this->webView, SLOT(setSelectionFont(QFont))
                );
    }

    if (this->fontsizebox)
    {
        connect(this->fontsizebox, SIGNAL(editTextChanged(QString)),
                SLOT(setSelectionFontSize(QString))
                );
    }
}

void MainWindow::disconnectFontControls()
{
    if (!this->webView)
    {
        qDebug() << "No webview";
        return;
    }

    if (this->fontbox)
    {
        disconnect(this->fontbox, SIGNAL(currentFontChanged(QFont)),
                   this->webView, SLOT(setSelectionFont(QFont))
                   );
    }

    if (this->fontsizebox)
    {
        disconnect(this->fontsizebox, SIGNAL(editTextChanged(QString)),
                   this, SLOT(setSelectionFontSize(QString))
                   );
    }
}

void MainWindow::setSelectionFontSize(const QString& selectedSize)
{
    bool ok;
    double fontSize = selectedSize.toDouble(&ok);
    if (ok)
        this->webView->setSelectionFontSize(fontSize);
    else
        qDebug() << "Unable to convert font size to double:  " << selectedSize;
}

void MainWindow::updateFontControls(const QString& fontFamily, double fontSize)
{
    // Prevent the control updates from triggering formatting
    this->disconnectFontControls();

    if (this->fontbox && !fontFamily.isEmpty())
    {
        QFont font;
        font.setFamily(fontFamily);
        this->fontbox->setCurrentFont(font);
    }

    if (this->fontsizebox && fontSize >= 1.0)
    {
        this->fontsizebox->setEditText(QString::number(fontSize));
    }

    // Re-enable font control connections
    QTimer::singleShot(0, this, SLOT(connectFontControls()));
}

void MainWindow::handleTray(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        #ifdef Q_OS_LINUX
        if (this->isVisible() && this->isActiveWindow())
        #else   // Loses focus on tray click when running in Windows
        if (this->isVisible())
        #endif
        {
            this->hide();
        }
        else
        {
            this->show();
            this->setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);    // necessary in Windows to raise window
            this->activateWindow();
        }
        break;

    default:
        // errr.... nothing
        break;
    }
}

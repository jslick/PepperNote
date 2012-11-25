#include "notewebview.h"
#include "utility.h"
#include "notebook.h"       // NotebookException
#include "notebookpage.h"

#include <QAction>
#include <QTime>
#include <QTimer>
#include <QWebFrame>
#include <QWebElement>
#include <QWebInspector>
#include <QDebug>

#define DEFAULT_SAVE_TIMER_INTERVAL (5000)
#define DEFAULT_SAVE_INTERVAL       (5000)

NoteWebView::NoteWebView(QWidget* parent) :
    QWebView(parent),
    currentPage(0),
    saveTimerInProgress(false),
    inspector(0)
{
    this->elapsedSave.invalidate();

    this->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    this->inspector = new QWebInspector;
    inspector->setPage(this->page());

    this->initActions();

    connect(this->page(), SIGNAL(contentsChanged()),
            this, SLOT(noteChanged())
            );
}

void NoteWebView::setPage(NotebookPage& page)
{
    this->currentPage = &page;
    this->showCurrentPage();
}

void NoteWebView::closing()
{
    if (this->inspector)
        this->inspector->setVisible(false);

    this->savePage();
}

void NoteWebView::toggleDevTools()
{
    this->inspector->setVisible(!this->inspector->isVisible());
}

void NoteWebView::noteChanged()
{
    if (!this->saveTimerInProgress)
    {   // prevent a ton of timers being created and triggering as the user types
        this->saveTimerInProgress = true;
        QTimer::singleShot(DEFAULT_SAVE_TIMER_INTERVAL, this, SLOT(checkSaveNote()));
    }
}

void NoteWebView::checkSaveNote()
{
    if (this->elapsedSave.isValid() == false ||
        this->elapsedSave.elapsed() > DEFAULT_SAVE_INTERVAL)
    {
        this->savePage();

        this->saveTimerInProgress = false;  // Let noteChange() know that it can set a new timer
    }
    else
    {
        QTimer::singleShot(DEFAULT_SAVE_TIMER_INTERVAL, this, SLOT(checkSaveNote()));
    }
}

void NoteWebView::setNoteContent()
{
    disconnect(this, SLOT(setNoteContent()));

    CHECK_POINTER_GUI(this->currentPage, "Could not load notebook page");

    // TODO:  Make getHtml asynchronous
    QString noteHtml = this->currentPage->isPersisted() ?
                       this->currentPage->getHtml() :
                       getFileUtf8(":/editor/html/new_note.html");

    QWebElement contentElement = this->page()->mainFrame()->findFirstElement("#note_content");
    contentElement.setInnerXml(noteHtml);

    this->page()->mainFrame()->evaluateJavaScript("setFocus('note_content', 1, true)");
}

void NoteWebView::savePage()
{
    qDebug() << '[' << QTime::currentTime() << "] Saving note";

    QWebElement contentElement = this->page()->mainFrame()->findFirstElement("#note_content");
    QString contentHtml = contentElement.toInnerXml();
    try
    {
        this->currentPage->saveContent(contentHtml);

        this->elapsedSave.restart();    // Delay next auto-save
    }
    catch (NotebookException& e)
    {
        showMessage(tr("Unable to save page"));
    }
}

void NoteWebView::initActions()
{
    QAction* saveAction = new QAction(tr("Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()),
            SLOT(savePage())
            );
    this->addAction(saveAction);

    QAction* devAction = new QAction(tr("Developer Tools"), this);
    devAction->setShortcut(tr("F12"));
    connect(devAction, SIGNAL(triggered()),
            SLOT(toggleDevTools())
            );
    this->addAction(devAction);

    QAction* boldAction = this->pageAction(QWebPage::ToggleBold);
    if (boldAction)
    {
        boldAction->setShortcut(QKeySequence::Bold);
        this->addAction(boldAction);
    }

    QAction* italicAction = this->pageAction(QWebPage::ToggleItalic);
    if (italicAction)
    {
        italicAction->setShortcut(QKeySequence::Italic);
        this->addAction(italicAction);
    }

    QAction* underlineAction = this->pageAction(QWebPage::ToggleUnderline);
    if (underlineAction)
    {
        underlineAction->setShortcut(QKeySequence::Underline);
        this->addAction(underlineAction);
    }

    QAction* ulAction = this->pageAction(QWebPage::InsertUnorderedList);
    if (ulAction)
    {
        ulAction->setIconText(tr("Bulleted List"));
        ulAction->setShortcut(tr("Shift+F12"));
        this->addAction(ulAction);
    }

    QAction* olAction = this->pageAction(QWebPage::InsertOrderedList);
    if (olAction)
    {
        olAction->setIconText(tr("Numbered List"));
        olAction->setShortcut(tr("Ctrl+Shift+F12"));
        this->addAction(olAction);
    }
}

void NoteWebView::showCurrentPage()
{
    connect(this, SIGNAL(loadFinished(bool)),
            SLOT(setNoteContent())
            );

    QString docHtml = getFileUtf8(":/editor/html/note.html");
    this->setHtml(docHtml, QUrl("qrc:/editor/"));
    // As a consequence of setHtml,
    // setNoteContent() slot will show the note contents
}

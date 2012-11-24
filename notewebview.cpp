#include "notewebview.h"
#include "utility.h"
#include "notebook.h"       // NotebookException
#include "notebookpage.h"

#include <QTime>
#include <QTimer>
#include <QWebFrame>
#include <QWebElement>
#include <QDebug>

#define DEFAULT_SAVE_TIMER_INTERVAL (5000)
#define DEFAULT_SAVE_INTERVAL       (5000)

NoteWebView::NoteWebView(QWidget* parent) :
    QWebView(parent),
    currentPage(0),
    saveTimerInProgress(false)
{
    this->elapsedSave.invalidate();

    connect(this->page(), SIGNAL(contentsChanged()),
            this, SLOT(noteChanged())
            );
}

void NoteWebView::setPage(NotebookPage& page)
{
    this->currentPage = &page;
    this->showCurrentPage();
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
        qDebug() << '[' << QTime::currentTime() << "] Saving note";
        this->savePage();

        this->elapsedSave.start();
        this->saveTimerInProgress = false;
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

void NoteWebView::savePage()
{
    QWebElement contentElement = this->page()->mainFrame()->findFirstElement("#note_content");
    QString contentHtml = contentElement.toInnerXml();
    try
    {
        this->currentPage->saveContent(contentHtml);
    }
    catch (NotebookException& e)
    {
        // TODO:  put e message in message
        showMessage(tr("Unable to save page"));
    }
}

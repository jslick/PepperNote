#include "notebookpage.h"
#include "notewebview.h"

#include <QTime>
#include <QTimer>
#include <QDebug>

#define DEFAULT_SAVE_TIMER_INTERVAL (5000)
#define DEFAULT_SAVE_INTERVAL       (5000)

NoteWebView::NoteWebView(QWidget* parent) :
    QWebView(parent),
    currentPage(0),
    saveTimerInProgress(false)
{
    connect(this->page(), SIGNAL(contentsChanged()),
            this, SLOT(noteChanged())
            );
}

void NoteWebView::setPage(NotebookPage* page)
{
    // TODO:  validate param

    // TODO:  How to deal with threading?  Maybe only event thread should be
    //        using this function

    this->currentPage = page;

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
        // TODO: save

        this->elapsedSave.start();
        this->saveTimerInProgress = false;
    }
    else
    {
        QTimer::singleShot(DEFAULT_SAVE_TIMER_INTERVAL, this, SLOT(checkSaveNote()));
    }
}

void NoteWebView::showCurrentPage()
{
    // TODO:  validate currentPage

    // TODO:  Make getHtml asynchronous
    // Implicit QString data sharing makes this efficient
    QString noteHtml = this->currentPage->getHtml();
    this->setHtml(noteHtml, QUrl("qrc:/editor/"));
}

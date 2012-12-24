#include "notewebview.h"
#include "utility.h"
#include "notebook.h"       // NotebookException
#include "notebookpage.h"
#include "javascriptapi.h"

#include <QAction>
#include <QTime>
#include <QTimer>
#include <QWebFrame>
#include <QWebElement>
#include <QWebInspector>
#include <QDebug>

#define DEFAULT_SAVE_TIMER_INTERVAL (5000)
#define DEFAULT_SAVE_INTERVAL       (5000)

NoteWebView::NoteWebView(JavascriptApi& jsApi, QWidget* parent) :
    QWebView(parent),
    jsApi(jsApi),
    currentNotebook(0), currentPage(0),
    saveTimerInProgress(false),
    inspector(0)
{
    this->elapsedSave.invalidate();

    this->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    this->inspector = new QWebInspector;
    inspector->setPage(this->page());

    this->page()->mainFrame()->addToJavaScriptWindowObject("Api", &this->jsApi);
    this->initActions();

    connect(this->page(), SIGNAL(contentsChanged()),
            this, SLOT(noteChanged())
            );
}

void NoteWebView::setPage(Notebook& notebook, NotebookPage& page)
{
    this->currentNotebook = &notebook;
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

void NoteWebView::setSelectionFont(const QFont& font)
{
    QString command = QString("document.execCommand(\"fontName\", false, \"%1\")").arg(font.family());
    this->page()->mainFrame()->evaluateJavaScript(command);
}

void NoteWebView::setSelectionFontSize(double fontSize)
{
    // TODO:  Create undo stack item
    //        The QWebPage does not recognize changes made by the JavaScript.

    // fontSize command doesn't work because it expects the ancient 1-7 font
    // size... where the hell is the command to set font size by point size?
    QString js = QString("setSelectionFontSize(%1)").arg(fontSize);
    QVariant result = this->page()->mainFrame()->evaluateJavaScript(js);
    // Since JavaScript changes to the document don't trigger the
    // contentsChanged signal, we must manually trigger save
    if (result.toBool())
        this->noteChanged();
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

    // get the note HTML from disk (or cache)
    QString noteHtml = this->currentNotebook->getPageContents(*this->currentPage);

    // Get set the main HTML structure that wraps the page's HTML
    QWebElement contentElement = this->page()->mainFrame()->findFirstElement("#note_content");
    contentElement.setInnerXml(noteHtml);

    this->page()->mainFrame()->evaluateJavaScript("setFocus('note_content', 1, true, notifySelectionChange)");
}

void NoteWebView::savePage()
{
    qDebug() << '[' << QTime::currentTime() << "] Saving note";

    Q_ASSERT(this->currentNotebook);
    Q_ASSERT(this->currentPage);

    QWebElement contentElement = this->page()->mainFrame()->findFirstElement("#note_content");
    QString contentHtml = contentElement.toInnerXml();
    try
    {
        this->currentNotebook->savePage(*this->currentPage, contentHtml);

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

    // TODO:  Expand tree to current page
}

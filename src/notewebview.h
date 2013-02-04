/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef NOTEWEBVIEW_H
#define NOTEWEBVIEW_H

#include <QWebView>
#include <QElapsedTimer>

class Notebook;
class NotebookPage;
class JavascriptApi;

class QFont;
class QWebInspector;

class NoteWebView : public QWebView
{
    Q_OBJECT
public:
    explicit NoteWebView(JavascriptApi& jsApi, QWidget* parent = 0);

    NotebookPage* getCurrentPage();

    void getCurrentPage(Notebook*& notebook, NotebookPage*& page);

    void setPage(Notebook& notebook, NotebookPage& page);

signals:

    void pageChanged(Notebook* notebook, NotebookPage* page);
    void fontChanged(const QString& fontFamily, int fontSize);

public slots:
    void savePage();        // Saves the view HTML to the current page
    void blankEditor();
    void closing();
    void toggleDevTools();

    void setSelectionFont(const QFont& font);
    void setSelectionFontSize(double fontSize);

private slots:
    void noteChanged();     // Starts a timer to save the current note
    void checkSaveNote();   // Check to see if it is time to save the current note

    void setNoteContent();  // Sets the contenteditable div contents after the document is loaded

private:
    void initActions();

    void showCurrentPage(); // Sets the view HTML content from this->currentPage

    inline QVariant evaluateJavaScript(const QString& script);  // Evaluates JavaScript in the main frame

    JavascriptApi&  jsApi;

    Notebook*       currentNotebook;
    NotebookPage*   currentPage;

    // use these members only in event thread
    bool            saveTimerInProgress;
    QElapsedTimer   elapsedSave;

    QWebInspector*  inspector;
};

#endif // NOTEWEBVIEW_H

#ifndef NOTEWEBVIEW_H
#define NOTEWEBVIEW_H

#include <QWebView>
#include <QElapsedTimer>

class NotebookPage;

class NoteWebView : public QWebView
{
    Q_OBJECT
public:
    explicit NoteWebView(QWidget* parent = 0);

    void setPage(NotebookPage& page);

signals:

public slots:

private slots:
    void noteChanged();     // Starts a timer to save the current note
    void checkSaveNote();   // Check to see if it is time to save the current note

    void setNoteContent();  // Sets the contenteditable div contents after the document is loaded

private:
    void showCurrentPage(); // Sets the view HTML content from this->currentPage

    // Event thread only
    void savePage();    // Saves the view HTML to the current page

    NotebookPage* currentPage;

    // use these members only in event thread
    bool            saveTimerInProgress;
    QElapsedTimer   elapsedSave;
};

#endif // NOTEWEBVIEW_H

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
    void noteChanged();
    void checkSaveNote();

    void setNoteContent();

private:
    void showCurrentPage();

    // Event thread only
    void savePage();

    NotebookPage* currentPage;

    // use these members only in event thread
    bool            saveTimerInProgress;
    QElapsedTimer   elapsedSave;
};

#endif // NOTEWEBVIEW_H

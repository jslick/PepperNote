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

    void setPage(NotebookPage* page);

signals:

public slots:

private slots:
    void noteChanged();
    void checkSaveNote();

private:
    void showCurrentPage();

    NotebookPage* currentPage;

    // use these members only in event thread
    bool            saveTimerInProgress;
    QElapsedTimer   elapsedSave;
};

#endif // NOTEWEBVIEW_H

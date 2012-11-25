#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QDir;
class Notebook;
class NoteWebView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    static QDir getNotebooksDirectory();
    static QString getDefaultNotebookFilename();

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void initToolbar();

    void loadNotebooks();

private slots:
    void showLoadedNotebook(Notebook* notebook);

private:
    Ui::MainWindow* ui;
    NoteWebView*    webView;

    // Event thread only (at this time)
    QList<Notebook*> loadedNotebooks;
};

#endif // MAINWINDOW_H

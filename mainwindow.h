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
    void loadNotebooks();

private slots:
    void showLoadedNotebook(Notebook* notebook);

private:
    Ui::MainWindow* ui;
    NoteWebView*    webView;
};

#endif // MAINWINDOW_H

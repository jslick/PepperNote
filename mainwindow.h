#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class NoteWebView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void setNewNote();

    Ui::MainWindow* ui;
    NoteWebView*    webView;
};

#endif // MAINWINDOW_H

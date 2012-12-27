/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QDir;
class QComboBox;
class QFontComboBox;
class Notebook;
class JavascriptApi;
class QMenu;
class NotebookTree;
class QTreeWidgetItem;
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

protected:

    void closeEvent(QCloseEvent* event);

private:
    void initMenubar();

    void initToolbar();

    void initNotebookTree();

    void loadNotebooks();

private slots:
    void showLoadedNotebook(Notebook* notebook);

    void createNewPage();

    /**
     * @brief Switch to a different page
     *
     * @param current   The tree item of the page to switch
     * @param previous  The previously selected tree item
     */
    void switchPage(QTreeWidgetItem* current, QTreeWidgetItem* previous);

    /**
     * @brief Set the page name of the page in the current web view
     *
     * @param pageTitle     The new page title
     */
    void setCurrentPageName(QString pageTitle);

    /**
     * @brief Enable connections that format the document
     *
     * When the controls are programmatically updated, they probably shouldn't
     * cause the document to be formatted.  These two functions are used when
     * programmatically updating the font controls.
     *
     * @see disconnectFontControls
     */
    void connectFontControls();

    /**
     * @brief Disable connections that format the document
     *
     * When the controls are programmatically updated, they probably shouldn't
     * cause the document to be formatted.  These two functions are used when
     * programmatically updating the font controls.
     *
     * @see connectFontControls()
     */
    void disconnectFontControls();

    /**
     * @brief Sets the font size of the current document selection
     *
     * @param selectedSize  Font size, in points
     */
    void setSelectionFontSize(const QString& selectedSize);

    /**
     * @brief Update the font controls with the font properties
     *
     * @param fontFamily
     * @param fontSize
     */
    void updateFontControls(const QString& fontFamily, double fontSize);

private:
    Ui::MainWindow* ui;
    JavascriptApi*  jsApi;
    // NOTE:  The documentation lies.  It says that each note should have an
    //        instance of JavascriptApi... but there is only this one.
    QMenu*          fileMenu;
    NoteWebView*    webView;
    NotebookTree*   notebookTree;
    QDockWidget*    notebookTreeDock;

    QFontComboBox*  fontbox;
    QComboBox*      fontsizebox;

    // Event thread only (at this time)
    QList<Notebook*> loadedNotebooks;
};

#endif // MAINWINDOW_H

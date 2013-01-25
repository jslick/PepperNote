/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef NOTEBOOKTREE_H
#define NOTEBOOKTREE_H

#include <QTreeWidget>

#include <QHash>
#include <QMenu>

class Notebook;
class NotebookPage;
class TreeNotebookItem;

class NotebookTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit NotebookTree(QWidget* parent = 0);

    void addNotebook(Notebook& notebook);

    void addPageItem(Notebook& notebook, const QString& sectionName, NotebookPage& page);

signals:

public slots:

    void selectPage(Notebook* notebook, NotebookPage* page);

protected:

    void contextMenuEvent(QContextMenuEvent* event);

private slots:

    void movePageUp();
    void movePageDown();
    void movePageToSection(QAction* action);
    void deletePageConfirmation();

private:

    enum MovePageDirection {
        MovePageUp   = -1,
        MovePageDown = 1
    };

    void movePage(MovePageDirection direction);

    void selectItem(QTreeWidgetItem* item);

    QHash<Notebook*,TreeNotebookItem*> notebookTreeItems;

    // Context menu stuff
    QMenu       pageContextMenu;
    QAction*    movePageUpAction;
    QAction*    movePageDownAction;
    QMenu       moveToSectionMenu;
    QAction*    deletePageAction;

};

#endif // NOTEBOOKTREE_H

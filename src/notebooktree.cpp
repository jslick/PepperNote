/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "notebooktree.h"

#include "lambdaguard.h"
#include "notebook.h"
#include "treenotebookitem.h"
#include "treenotebookpageitem.h"
#include "notebookexception.h"

#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>

NotebookTree::NotebookTree(QWidget* parent) :
    QTreeWidget(parent)
{
    this->setColumnCount(1);
    this->header()->hide();

    movePageUpAction = new QAction(QObject::tr("Move Up"), this);
    movePageDownAction = new QAction(QObject::tr("Move Down"), this);
    connect(movePageUpAction, SIGNAL(triggered()),
            SLOT(movePageUp())
            );
    connect(movePageDownAction, SIGNAL(triggered()),
            SLOT(movePageDown())
            );
    this->pageContextMenu.addAction(movePageUpAction);
    this->pageContextMenu.addAction(movePageDownAction);
}

void NotebookTree::addNotebook(Notebook& notebook)
{
    TreeNotebookItem* treeItem = new TreeNotebookItem(notebook);
    if (this->notebookTreeItems.find(&notebook) == this->notebookTreeItems.end())
    {
        this->notebookTreeItems[&notebook] = treeItem;
        this->addTopLevelItem(treeItem);
    }
    else
    {
        delete treeItem;
        throw NotebookException("Cannot add a notebook multiple times to the tree");
    }
}

void NotebookTree::addPageItem(Notebook& notebook, const QString& sectionName, NotebookPage& page)
{

    auto it = this->notebookTreeItems.find(&notebook);
    if (it == this->notebookTreeItems.end())
        throw NotebookException("Cannot add page to tree; notebook not found");
    else
        (*it)->addPageItem(sectionName, page);
}

void NotebookTree::selectPage(Notebook* notebook, NotebookPage* page)
{
    auto notebookTreeIter = this->notebookTreeItems.find(notebook);
    if (notebookTreeIter == this->notebookTreeItems.end())
        throw NotebookException("Could not find tree node for the page to select");

    TreeNotebookItem* notebookTree = *notebookTreeIter;
    QTreeWidgetItem* sectionTree = 0;
    TreeNotebookPageItem* pageNode = 0;
    notebookTree->getPathToPage(*page, sectionTree, pageNode);
    if (!sectionTree)
        throw NotebookException("Result from getPathToPage; sectionTree is null");
    if (!pageNode)
        throw NotebookException("Result from getPathToPage; pageNode is null");

    notebookTree->setExpanded(true);
    sectionTree->setExpanded(true);
    this->selectItem(pageNode);
}

void NotebookTree::contextMenuEvent(QContextMenuEvent* event)
{
    QList<QTreeWidgetItem*> selectedList = this->selectedItems();
    if (selectedList.length() >= 1)
    {
        QTreeWidgetItem* selected = selectedList.at(0);
        if (dynamic_cast<TreeNotebookPageItem*>( selected ))
        {
            // Disable actions on boundaries
            QTreeWidgetItem* selectedParent = selected->parent();
            Q_ASSERT(selectedParent);
            int index = selectedParent->indexOfChild(selected);
            this->movePageUpAction->setEnabled(index > 0);
            this->movePageDownAction->setEnabled(index < selectedParent->childCount() - 1);

            this->pageContextMenu.exec(event->globalPos());
        }
    }

    QTreeWidget::contextMenuEvent(event);
}

void NotebookTree::movePageUp()
{
    this->movePage(NotebookTree::MovePageUp);
}

void NotebookTree::movePageDown()
{
    this->movePage(NotebookTree::MovePageDown);
}

void NotebookTree::movePage(NotebookTree::MovePageDirection direction)
{
    QList<QTreeWidgetItem*> selectedList = this->selectedItems();
    if (selectedList.length() >= 1)
    {
        this->blockSignals(true);   // Don't change page when deselecting item
        LambdaGuard unblockSignals([this]{ // RAII:  Unblock signals, even on returns and exceptions
            this->blockSignals(false);
        });
        Q_UNUSED(unblockSignals); // until GCC 4.8.0 - http://gcc.gnu.org/bugzilla/show_bug.cgi?id=10416#c13

        QTreeWidgetItem* selected = selectedList.at(0);
        QTreeWidgetItem* selectedParent = selected->parent();
        if (!selectedParent)
            return;

        int index = selectedParent->indexOfChild(selected);

        switch (direction)
        {
        case NotebookTree::MovePageUp:
            if (index <= 0)
                return;
            index--;
            break;

        case NotebookTree::MovePageDown:
            if (index >= selectedParent->childCount() - 1)
                return;
            index++;
            break;
        }
        Q_ASSERT(index >= 0);

        // Move item
        selectedParent->removeChild(selected);
        selectedParent->insertChild(index, selected);
        this->selectItem(selected);

        unblockSignals.doNow();

        // Let the Notebook know that pages were swapped
        if (TreeNotebookPageItem* pageItem = dynamic_cast<TreeNotebookPageItem*>( selected ))
            pageItem->getNotebook().movePage(pageItem->getNotebookPage(), direction);
    }
}

void NotebookTree::selectItem(QTreeWidgetItem* item)
{
    Q_ASSERT(item);

    // Deselect all currently selected items in the tree
    for (QTreeWidgetItem* item : this->selectedItems())
    {
        item->setSelected(false);
    }

    item->setSelected(true);
}

/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "notebooktree.h"

#include "notebook.h"
#include "treenotebookitem.h"
#include "treenotebookpageitem.h"
#include "notebookexception.h"

#include <QHeaderView>

NotebookTree::NotebookTree(QWidget* parent) :
    QTreeWidget(parent)
{
    this->setColumnCount(1);
    this->header()->hide();
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

    // Deselect all currently selected items in the tree
    for (QTreeWidgetItem* item : this->selectedItems())
    {
        item->setSelected(false);
    }

    notebookTree->setExpanded(true);
    sectionTree->setExpanded(true);
    pageNode->setSelected(true);
}

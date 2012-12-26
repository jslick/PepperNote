/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef TREENOTEBOOKPAGEITEM_H
#define TREENOTEBOOKPAGEITEM_H

#include <QTreeWidgetItem>

class TreeNotebookItem;

class Notebook;
class NotebookPage;

/**
 * @brief A QTreeWidgetItem for notebook pages in the notebook tree
 *
 * The tree is like this:
 *  * Notebook
 *  ** Notebook section
 *  *** Notebook page (this class)
 */
class TreeNotebookPageItem : public QTreeWidgetItem
{
public:
    TreeNotebookPageItem(TreeNotebookItem& parent, NotebookPage& page);

    Notebook& getNotebook();

    NotebookPage& getNotebookPage();

private:
    TreeNotebookItem&   parent;
    NotebookPage&       page;
};

#endif // TREENOTEBOOKPAGEITEM_H

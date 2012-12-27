/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "treenotebookpageitem.h"

#include "notebookpage.h"
#include "treenotebookitem.h"

TreeNotebookPageItem::TreeNotebookPageItem(TreeNotebookItem& parent, NotebookPage& page)
    : parent(parent), page(page)
{
    this->setText(0, page.getName());

    connect(&page, SIGNAL(pageNameChanged(QString)),
            SLOT(updateText(QString))
            );
}

Notebook& TreeNotebookPageItem::getNotebook()
{
    return this->parent.notebook;
}

NotebookPage& TreeNotebookPageItem::getNotebookPage()
{
    return this->page;
}

void TreeNotebookPageItem::updateText(const QString& pageName)
{
    this->setText(0, pageName);
}

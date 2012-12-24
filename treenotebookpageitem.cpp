#include "treenotebookpageitem.h"

#include "treenotebookitem.h"

TreeNotebookPageItem::TreeNotebookPageItem(TreeNotebookItem& parent, NotebookPage& page)
    : parent(parent), page(page)
{
}

Notebook& TreeNotebookPageItem::getNotebook()
{
    return this->parent.notebook;
}

NotebookPage& TreeNotebookPageItem::getNotebookPage()
{
    return this->page;
}

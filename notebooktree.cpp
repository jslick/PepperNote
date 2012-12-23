#include "notebooktree.h"
#include "treenotebookitem.h"

NotebookTree::NotebookTree(QWidget* parent) :
    QTreeWidget(parent)
{
    this->setColumnCount(1);
}

void NotebookTree::addNotebook(Notebook& notebook)
{
    TreeNotebookItem* treeItem = new TreeNotebookItem(notebook);
    this->addTopLevelItem(treeItem);
}

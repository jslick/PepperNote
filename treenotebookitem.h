#ifndef TREENOTEBOOKITEM_H
#define TREENOTEBOOKITEM_H

#include <QTreeWidgetItem>

class Notebook;
class NotebookPage;

class TreeNotebookItem : public QTreeWidgetItem
{
public:
    explicit TreeNotebookItem(Notebook& notebook);

    void getPathToPage(NotebookPage* page, QTreeWidgetItem*& sectionTree, QTreeWidgetItem*& pageNode);

signals:

public slots:

private:
    Notebook& notebook;

    QHash<NotebookPage*,QPair<QTreeWidgetItem*,QTreeWidgetItem*> > sectionTrees;
    // NOTE:  This data structure would cause problems if pages were to be
    //        shared among sections in the same notebook.

};

#endif // TREENOTEBOOKITEM_H

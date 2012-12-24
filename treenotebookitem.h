#ifndef TREENOTEBOOKITEM_H
#define TREENOTEBOOKITEM_H

#include <QTreeWidgetItem>

class Notebook;
class NotebookPage;
class TreeNotebookPageItem;

/**
 * @brief A QTreeWidgetItem for notebooks (top-level) in the notebook tree
 *
 * The tree is like this:
 *  * Notebook (this class)
 *  ** Notebook section
 *  *** Notebook page
 */
class TreeNotebookItem : public QTreeWidgetItem
{
public:
    explicit TreeNotebookItem(Notebook& notebook);

    void getPathToPage(NotebookPage* page, QTreeWidgetItem*& sectionTree, TreeNotebookPageItem*& pageNode);

    Notebook& notebook;

signals:

public slots:

private:

    QHash<NotebookPage*,QPair<QTreeWidgetItem*,TreeNotebookPageItem*> > sectionTrees;
    // NOTE:  This data structure would cause problems if pages were to be
    //        shared among sections in the same notebook.

};

#endif // TREENOTEBOOKITEM_H

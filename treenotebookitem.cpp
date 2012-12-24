#include "treenotebookitem.h"

#include "notebook.h"
#include "notebookpage.h"
#include "treenotebookpageitem.h"
#include "notebookexception.h"

#include <QObject>

TreeNotebookItem::TreeNotebookItem(Notebook& notebook) :
    QTreeWidgetItem(), notebook(notebook)
{
    this->setText(0, QObject::tr("Notebook")); // TODO:  Notebook::getTitle()

    for (const QString& sectionName : notebook.getSectionNames())
    {
        QTreeWidgetItem* sectionItem = new QTreeWidgetItem;
        sectionItem->setText(0, sectionName);
        this->addChild(sectionItem);

        for (NotebookPage* page : notebook.getPages(sectionName))
        {
            Q_ASSERT(page);

            TreeNotebookPageItem* pageItem = new TreeNotebookPageItem(*this, *page);
            pageItem->setText(0, page->getName());
            sectionItem->addChild(pageItem);

            // Save the path to this page
            if (this->sectionTrees.find(page) == this->sectionTrees.end())
                this->sectionTrees[page] = qMakePair(sectionItem, pageItem);
            else
                qDebug() << "Notebook page already in notebook tree; skipping insertion of subsequent instances";
        }
    }
}

void TreeNotebookItem::getPathToPage(NotebookPage* page, QTreeWidgetItem*& sectionTree, TreeNotebookPageItem*& pageNode)
{
    if (!page)
        throw NotebookException("Cannot get tree path to null page");

    auto it = this->sectionTrees.find(page);
    if (it == this->sectionTrees.end())
        throw NotebookException("Could not find tree path to notebook page");

    auto pair = *it;
    sectionTree = pair.first;
    pageNode = pair.second;
}

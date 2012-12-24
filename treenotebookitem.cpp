#include "treenotebookitem.h"
#include "notebook.h"
#include "notebookpage.h"

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
            QTreeWidgetItem* pageItem = new QTreeWidgetItem;
            pageItem->setText(0, page->getName());
            sectionItem->addChild(pageItem);
        }
    }
}

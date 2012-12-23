#include "treenotebookitem.h"
#include "notebook.h"

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

        for (const QString& pageName : notebook.getPageNames(sectionName))
        {
            QTreeWidgetItem* pageItem = new QTreeWidgetItem;
            pageItem->setText(0, pageName);
            sectionItem->addChild(pageItem);
        }
    }
}

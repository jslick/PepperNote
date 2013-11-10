/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "treenotebookitem.h"

#include "notebook.h"
#include "treenotebookpageitem.h"
#include "notebookexception.h"

#include <QObject>

TreeNotebookItem::TreeNotebookItem(Notebook& notebook) :
    QObject(), QTreeWidgetItem(QTreeWidgetItem::Type), notebook(notebook)
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
            this->addPageToSectionItem(*sectionItem, *page);
        }
    }

    connect(&this->notebook, SIGNAL(pageMoved(NotebookPage*,QString,int)),
            SLOT(updatePageItem(NotebookPage*,QString,int))
            );
    connect(&this->notebook, SIGNAL(pageRemoved(QString,NotebookPage*)),
            SLOT(removePageItem(QString,NotebookPage*))
            );
}

void TreeNotebookItem::getPathToPage(NotebookPage& page, QTreeWidgetItem*& sectionTree, TreeNotebookPageItem*& pageNode)
{
    auto it = this->sectionTrees.find(&page);
    if (it == this->sectionTrees.end())
        throw NotebookException("Could not find tree path to notebook page");

    auto pair = *it;
    sectionTree = pair.first;
    pageNode = pair.second;
}

void TreeNotebookItem::addPageItem(const QString& sectionName, NotebookPage& page)
{
    QTreeWidgetItem& sectionItem = this->findOrCreateSectionItem(sectionName);
    this->addPageToSectionItem(sectionItem, page);
}

void TreeNotebookItem::updatePageItem(NotebookPage* page, QString sectionName, int index)
{
    Q_ASSERT(page);

    QTreeWidgetItem* sectionTree = 0;
    TreeNotebookPageItem* pageNode = 0;
    this->getPathToPage(*page, sectionTree, pageNode);
    Q_ASSERT(sectionTree);
    Q_ASSERT(pageNode);

    QTreeWidgetItem& newSectionTree = this->findOrCreateSectionItem(sectionName);

    sectionTree->removeChild(pageNode);
    newSectionTree.insertChild(index, pageNode);
    // update tree path index
    this->sectionTrees[page].first = &newSectionTree;

    // TODO:  Prevent change of selection when moving nodes
}

void TreeNotebookItem::removePageItem(QString sectionName, NotebookPage* page)
{
    Q_UNUSED(sectionName);
    Q_ASSERT(page);

    QTreeWidgetItem* sectionTree = 0;
    TreeNotebookPageItem* pageNode = 0;
    this->getPathToPage(*page, sectionTree, pageNode);
    Q_ASSERT(sectionTree);
    Q_ASSERT(pageNode);

    sectionTree->removeChild(pageNode);
}

QTreeWidgetItem& TreeNotebookItem::findOrCreateSectionItem(const QString& sectionName)
{
    for (int i = 0; i < this->childCount(); i++)
    {
        QTreeWidgetItem* sectionItem = this->child(i);
        Q_ASSERT(sectionItem);

        if (sectionItem->text(0) == sectionName)
            return *sectionItem;
    }

    QTreeWidgetItem* sectionItem = new QTreeWidgetItem;
    sectionItem->setText(0, sectionName);
    this->addChild(sectionItem);

    return *sectionItem;
}

void TreeNotebookItem::addPageToSectionItem(QTreeWidgetItem& sectionItem, NotebookPage& page)
{
    TreeNotebookPageItem* pageItem = new TreeNotebookPageItem(*this, page);
    sectionItem.addChild(pageItem);

    // Save the path to this page
    if (this->sectionTrees.find(&page) == this->sectionTrees.end())
        this->sectionTrees[&page] = qMakePair(&sectionItem, pageItem);
    else
        qDebug() << "Notebook page already in notebook tree; skipping insertion of subsequent instances";
}

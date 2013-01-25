/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "notebooktree.h"

#include "lambdaguard.h"
#include "notebook.h"
#include "notebookpage.h"
#include "treenotebookitem.h"
#include "treenotebookpageitem.h"
#include "notebookexception.h"

#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>

class MoveToSectionAction : public QAction
{
    Q_OBJECT
public:
    Notebook&       notebook;
    QString         sectionName;
    NotebookPage&   page;

    MoveToSectionAction(Notebook& notebook, const QString& sectionName, NotebookPage& page, QWidget* parent = 0)
        : QAction(sectionName, parent),
          notebook(notebook), sectionName(sectionName), page(page)
    { }
};
#include "notebooktree.moc"

NotebookTree::NotebookTree(QWidget* parent) :
    QTreeWidget(parent),
    moveToSectionMenu(tr("Move to section"))
{
    this->setColumnCount(1);
    this->header()->hide();

    this->movePageUpAction = new QAction(QObject::tr("Move Up"), this);
    this->movePageDownAction = new QAction(QObject::tr("Move Down"), this);
    connect(this->movePageUpAction, SIGNAL(triggered()),
            SLOT(movePageUp())
            );
    connect(this->movePageDownAction, SIGNAL(triggered()),
            SLOT(movePageDown())
            );
    this->pageContextMenu.addAction(this->movePageUpAction);
    this->pageContextMenu.addAction(this->movePageDownAction);
    this->pageContextMenu.addSeparator();
    this->pageContextMenu.addMenu(&this->moveToSectionMenu);
    connect(&this->moveToSectionMenu, SIGNAL(triggered(QAction*)),
            SLOT(movePageToSection(QAction*))
            );
    this->deletePageAction = new QAction(QObject::tr("Delete Page"), this);
    this->pageContextMenu.addSeparator();
    this->pageContextMenu.addAction(this->deletePageAction);
    connect(this->deletePageAction, SIGNAL(triggered()),
            SLOT(deletePageConfirmation())
            );
}

void NotebookTree::addNotebook(Notebook& notebook)
{
    TreeNotebookItem* treeItem = new TreeNotebookItem(notebook);
    if (this->notebookTreeItems.find(&notebook) == this->notebookTreeItems.end())
    {
        this->notebookTreeItems[&notebook] = treeItem;
        this->addTopLevelItem(treeItem);
    }
    else
    {
        delete treeItem;
        throw NotebookException("Cannot add a notebook multiple times to the tree");
    }
}

void NotebookTree::addPageItem(Notebook& notebook, const QString& sectionName, NotebookPage& page)
{

    auto it = this->notebookTreeItems.find(&notebook);
    if (it == this->notebookTreeItems.end())
        throw NotebookException("Cannot add page to tree; notebook not found");
    else
        (*it)->addPageItem(sectionName, page);
}

void NotebookTree::selectPage(Notebook* notebook, NotebookPage* page)
{
    auto notebookTreeIter = this->notebookTreeItems.find(notebook);
    if (notebookTreeIter == this->notebookTreeItems.end())
        throw NotebookException("Could not find tree node for the page to select");

    TreeNotebookItem* notebookTree = *notebookTreeIter;
    QTreeWidgetItem* sectionTree = 0;
    TreeNotebookPageItem* pageNode = 0;
    notebookTree->getPathToPage(*page, sectionTree, pageNode);
    if (!sectionTree)
        throw NotebookException("Result from getPathToPage; sectionTree is null");
    if (!pageNode)
        throw NotebookException("Result from getPathToPage; pageNode is null");

    notebookTree->setExpanded(true);
    sectionTree->setExpanded(true);
    this->selectItem(pageNode);
}

void NotebookTree::contextMenuEvent(QContextMenuEvent* event)
{
    QList<QTreeWidgetItem*> selectedList = this->selectedItems();
    if (selectedList.length() >= 1)
    {
        QTreeWidgetItem* selected = selectedList.at(0);
        if (TreeNotebookPageItem* selectedPageItem = dynamic_cast<TreeNotebookPageItem*>( selected ))
        {
            // Disable actions on boundaries
            QTreeWidgetItem* selectedParent = selected->parent();
            Q_ASSERT(selectedParent);

            Notebook& notebook = selectedPageItem->getNotebook();
            NotebookPage& notebookPage = selectedPageItem->getNotebookPage();

            int index = selectedParent->indexOfChild(selected);
            this->movePageUpAction->setEnabled(index > 0);
            this->movePageDownAction->setEnabled(index < selectedParent->childCount() - 1);

            // Populate Move to Section menu
            this->moveToSectionMenu.clear();
            QStringList sectionNames = notebook.getSectionNames();
            for (const QString& sectionName : sectionNames)
            {
                MoveToSectionAction* action = new MoveToSectionAction(notebook, sectionName, notebookPage);
                this->moveToSectionMenu.addAction(action);
            }

            this->pageContextMenu.exec(event->globalPos());
        }
    }

    QTreeWidget::contextMenuEvent(event);
}

void NotebookTree::movePageUp()
{
    this->movePage(NotebookTree::MovePageUp);
}

void NotebookTree::movePageDown()
{
    this->movePage(NotebookTree::MovePageDown);
}

void NotebookTree::movePageToSection(QAction* action)
{
    Q_ASSERT(action);

    MoveToSectionAction* moveAction = dynamic_cast<MoveToSectionAction*>( action );
    Q_ASSERT(moveAction);

    moveAction->notebook.movePageToSection(moveAction->page, moveAction->sectionName);
}

void NotebookTree::deletePageConfirmation()
{
    QList<QTreeWidgetItem*> selectedList = this->selectedItems();
    if (selectedList.length() < 1)
        return;

    QTreeWidgetItem* selected = selectedList.at(0);
    TreeNotebookPageItem* pageItem = dynamic_cast<TreeNotebookPageItem*>( selected );
    if (!pageItem)
        return;

    NotebookPage& page = pageItem->getNotebookPage();
    const QString sectionName = pageItem->getNotebook().getPageSection(page);

    int retval = QMessageBox::question(
                     this,
                     tr("Delete Page"),
                     tr("Are you sure you want to delete the page %1?").arg(page.getName()),
                     QMessageBox::Yes | QMessageBox::No
                     );
    if (retval == QMessageBox::Yes)
        pageItem->getNotebook().removePage(sectionName, page);
}

void NotebookTree::movePage(NotebookTree::MovePageDirection direction)
{

    QList<QTreeWidgetItem*> selectedList = this->selectedItems();
    if (selectedList.length() >= 1)
    {
        QTreeWidgetItem* selected = selectedList.at(0);
        if (TreeNotebookPageItem* pageItem = dynamic_cast<TreeNotebookPageItem*>( selected ))
            pageItem->getNotebook().movePage(pageItem->getNotebookPage(), direction);
    }
}

void NotebookTree::selectItem(QTreeWidgetItem* item)
{
    Q_ASSERT(item);

    // Deselect all currently selected items in the tree
    for (QTreeWidgetItem* item : this->selectedItems())
    {
        item->setSelected(false);
    }

    item->setSelected(true);
}

/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

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
class TreeNotebookItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    explicit TreeNotebookItem(Notebook& notebook);

    /**
     * @brief Get the section item and page item of the specified page
     *
     * @param page          The page to search for in the tree
     * @param sectionTree   [out] Returns the section item that the tree is in
     * @param pageNode      [out] Returns the page item that represents the page
     */
    void getPathToPage(NotebookPage& page, QTreeWidgetItem*& sectionTree, TreeNotebookPageItem*& pageNode);

    /**
     * @brief Add a page item to the tree
     *
     * @param sectionName
     * @param page
     */
    void addPageItem(const QString& sectionName, NotebookPage& page);

    Notebook& notebook;

signals:

public slots:

private slots:

    // Move items when they are moved in the Notebook
    void updatePageItem(NotebookPage* page, QString sectionName, int index);

private:

    QTreeWidgetItem& findOrCreateSectionItem(const QString& sectionName);

    /**
     * @brief Add a tree item for the page in the specified section
     *
     * @param sectionItem   Section item to place the new page item in
     * @param page          The page to create a page item for
     */
    void addPageToSectionItem(QTreeWidgetItem& sectionItem, NotebookPage& page);

    QHash<NotebookPage*,QPair<QTreeWidgetItem*,TreeNotebookPageItem*> > sectionTrees;
    // NOTE:  This data structure would cause problems if pages were to be
    //        shared among sections in the same notebook.

};

#endif // TREENOTEBOOKITEM_H

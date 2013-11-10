/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QObject>

class NotebookFormat;
class NotebookPage;

class Notebook : public QObject
{
    Q_OBJECT
public:
    /**
     * @param fileFormat    The NotebookFormat used to save this notebook
     * @param parent        Parent QObject.  Can be NULL.
     */
    explicit Notebook(NotebookFormat* fileFormat, QObject* parent = 0);

    ~Notebook();

    /**
     * @param pageId
     * @return True if the page is persisted to disk
     */
    bool isPagePersisted(const QString& pageId) const;

    /**
     * @return The names of all sections in the notebook, in order.
     */
    QStringList getSectionNames() const;

    /**
     * @return All pages in the given section, in order
     */
    QList<NotebookPage*> getPages(const QString& sectionName) const;

    /**
     * @return The first page from the first section in the notebook.  NULL if
     *         there are no sections in the notebook.
     */
    NotebookPage* getFirstPage();

    /**
     * @param page
     * @return The persisted HTML contents of the page
     */
    QString getPageContents(NotebookPage& page) const;

    /**
     * @param pageId
     * @return The persisted HTML contents of the page
     */
    QString getPageContents(const QString& pageId) const;

    /**
     * @brief Adds the given page to the notebook
     *
     * @param sectionName
     * @param page
     *
     * @note This method does not save anything to the disk.
     * @note emits void pageAdded(QString section, NotebookPage* page).
     */
    void addPage(const QString& sectionName, NotebookPage* page);

    /**
     * @brief Saves the page to disk
     *
     * @param page
     * @param html  The HTML contents of the note
     */
    void savePage(NotebookPage& page, const QString& html);

    /**
     * @param page
     * @return Name of section that the page belongs to
     */
    QString getPageSection(const NotebookPage& page) const;

    /**
     * @brief Move a page within a section
     *
     * @param page
     * @param places    Number of places to move a page.  Negative numbers will
     *                  move the page u.
     */
    void movePage(NotebookPage& page, int places);

    /**
     * @brief Move a page to the end of another section
     *
     * @param page
     * @param sectionName
     */
    void movePageToSection(NotebookPage& page, const QString& sectionName);

    /**
     * @brief Remove a page from a scetion; updates disk to remove page
     *
     * @param sectionName
     * @param page
     */
    void removePage(const QString& sectionName, NotebookPage& page);

    /**
     * @brief Rename a section
     *
     * @param oldName
     * @param newName
     *
     * @return Success or failure
     */
    bool renameSection(const QString& oldName, const QString& newName);

signals:

    void pageAdded(QString section, NotebookPage* page);
    void pageMoved(NotebookPage* page, QString sectionName, int index);
    void pageRemoved(QString section, NotebookPage* page);

    void sectionRenamed(QString oldName, QString newName);

public slots:

private:
    typedef struct {
        QString                 name;
        QList<NotebookPage*>    pages;
    } Section;

    Section* findSection(const QString& sectionName);

    Section* findSection(const NotebookPage& page);

    const Section* findSection(const NotebookPage& page) const;

    Section& findOrCreateSection(const QString& sectionName);

    NotebookFormat* fileFormat;

    QList<Section> sections;
};

#endif // NOTEBOOK_H

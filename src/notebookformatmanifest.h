/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef NOTEBOOKFORMATMANIFEST_H
#define NOTEBOOKFORMATMANIFEST_H

#include <QObject>
#include <QList>
#include <QHash>

class NotebookFormatManifest : public QObject
{
    Q_OBJECT
public:
    explicit NotebookFormatManifest(QObject* parent = 0);

    /**
     * @brief Parse JSON string into data structures
     *
     * @param manifestJson  JSON string
     */
    void parseJson(const QString& manifestJson);

    /**
     * @brief Serialize manifest into JSON string
     *
     * @return JSON string
     */
    QString serialize() const;

    /**
     * @return Names of all sections in the manifest, in order that they are
     *         defined in the manifest
     */
    QStringList getSectionNames() const;

    /**
     * @brief Get a list of pages within a section, in the order that they are
     *        defined in the manifest
     *
     * @param sectionName
     * @param pageInfos     List of pages.  Each element is a pair of the page
     *                      ID and the page name.
     * @return Success or failure
     */
    bool getPages(const QString& sectionName, QList<QPair<QString,QString> >& pageInfos) const;

    /**
     * @param sectionIndex
     * @param pageIndex
     *
     * @return Page ID of the ith section and jth page
     */
    QString getPageId(int sectionIndex, int pageIndex) const;

    /**
     * @param pageId
     *
     * @return True if the manifest contains the specified page
     */
    bool containsPage(const QString& pageId) const;

    /**
     * @brief Add a new page to the manifest
     *
     * This function does not care if a page is added multiple times
     *
     * @param sectionName   Section that the page should belong in
     * @param pageId        ID of the new page.  Cannot be empty.
     * @param pageName      Name of the page
     * @pre The pageId cannot be empty.
     * @pre The page ID must be unique
     */
    void addPage(const QString& sectionName, const QString& pageId, const QString& pageName);

    /**
     * @param pageId
     *
     * @return Name of the page with the specified page ID
     */
    const QString& getPageName(const QString& pageId) const;

    /**
     * @param pageId
     * @param pageName
     */
    void setPageName(const QString& pageId, const QString& pageName);

    /**
     * @brief Reorder a page within its section in the manifest
     *
     * @param pageId    ID of the page to move
     * @param places    The number of places to move.  Negative value to move up.
     * @pre `places` should move the page to a valid index.
     */
    void movePage(const QString& pageId, int places);

    /**
     * @brief Move a page to the end of another section in the manifest
     *
     * @param pageId
     * @param sectionName
     * @pre Section should already exist
     */
    void movePageToSection(const QString& pageId, const QString& sectionName);

    /**
     * @brief Remove a page from a section
     *
     * @param sectionName
     * @param pageId
     */
    void removePage(const QString& sectionName, const QString& pageId);

signals:

public slots:

private:
    typedef struct {
        QString     id;
        QString     name;
    } Page;

    typedef struct {
        QString         name;
        QList<Page*>    pages;
    } Section;

    Section& findOrCreateSection(const QString& sectionName);

    QList<Page>             pages;
    QList<Section>          sections;
    QHash<QString,Section*> sectionIndex;   // section name <-> struct Section
    QHash<QString,Page*>    pageIndex;      // page id <-> struct Page
};

#endif // NOTEBOOKFORMATMANIFEST_H

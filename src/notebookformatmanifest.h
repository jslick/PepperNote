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

    void parseJson(const QString& manifestJson);

    QString serialize() const;

    QStringList getSectionNames() const;

    bool getPages(const QString& sectionName, QList<QPair<QString,QString> >& pageInfos) const;

    QString getPageId(int sectionIndex, int pageIndex) const;

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

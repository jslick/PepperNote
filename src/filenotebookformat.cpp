/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "filenotebookformat.h"
#include "utility.h"
#include "notebook.h"
#include "notebookpage.h"
#include "notebookexception.h"

#include <QStringBuilder>
#include <QDir>
#include <json.h>

FileNotebookFormat::FileNotebookFormat(const QString& filename, Notebook* parent) :
    NotebookFormat(parent),
    filename(filename)
{
}

void FileNotebookFormat::load()
{
    // Load page and section information from manifest JSON
    const QString manifestJson = getFileUtf8(this->filename % "/manifest.json");
    bool ok;
    QVariantMap result = QtJson::parse(manifestJson, ok).toMap();
    if (!ok)
        throw NotebookException("Unable to parse manifest");

    // Index page names
    QHash<QString,QString> pageNameIndex; // maps page id <=> page name
    const QVariantList pagesList = result["pages"].toList();
    for (const QVariant& pageMapVariant : pagesList)
    {
        const QVariantMap pageMap = pageMapVariant.toMap();

        const QString& pageId = pageMap["id"].toString();
        const QString& pageName = pageMap["name"].toString();
        if (pageId.length())
            pageNameIndex[pageId] = pageName;
    }

    // Find the actual pages in the manifest, and add them to the notebook
    const QVariantList sectionsList = result["sections"].toList();
    for (const QVariant& sectionMapVariant : sectionsList)
    {
        const QVariantMap sectionMap = sectionMapVariant.toMap();

        const QString& sectionName = sectionMap["name"].toString();
        const QStringList sectionPageList = sectionMap["pages"].toStringList();

        for (const QString& pageId : sectionPageList)
        {
            const QString& pageName = pageNameIndex[pageId];
            if (pageName.length())
            {
                NotebookPage* page = new NotebookPage(pageId, pageName);
                this->notebook->addPage(sectionName, page);
            }
            else
                qDebug() << "While loading the notebook manifest:  Page does not have a name";
        }
    }
}

bool FileNotebookFormat::isPagePersisted(const QString& pageId) const
{
    QString pageFilename = this->getPageFilename(pageId);
    return QFile::exists(pageFilename);
}

QString FileNotebookFormat::getPageContents(const QString& pageId) const
{
    QString pageFilename = getPageFilename(pageId);
    try
    {
        return getFileUtf8(pageFilename);
    }
    catch (... /* TODO */)
    {
        throw NotebookException("Unabled to open file " % pageFilename % " for reading");
    }
}

void FileNotebookFormat::savePage(Notebook& notebook, NotebookPage& page, const QString& html)
{
    Q_UNUSED(notebook);

    QDir notebookDir(this->filename);
    if (notebookDir.exists() == false)
        notebookDir.mkpath(".");

    QDir pagesDir = QDir(this->filename % "/pages");
    if (pagesDir.exists() == false)
        pagesDir.mkpath(".");

    // Make sure page has ID; new pages won't have one yet
    if (page.getId().isEmpty())
        page.createId();

    Q_ASSERT_X(page.getId().isEmpty() == false, "FileNotebookFormat::savePage", "Page ID cannot be null");

    QString pageFilename = pagesDir.path() % '/' % page.getId() % ".html";
    QFile pageFile(pageFilename);
    bool success = pageFile.open(QIODevice::WriteOnly);
    if (!success)
        throw NotebookException("Unabled to open file " % pageFilename % " for writing");

    qint64 numWritten = pageFile.write(html.toUtf8());
    Q_UNUSED(numWritten);

    this->saveManifest();

    pageFile.close();
}

void FileNotebookFormat::saveManifest()
{
    const QString serializedManifest = this->serializeManifest();

    QString manifestFilename = this->filename % "/manifest.json";
    QFile manifestFile(manifestFilename);
    bool success = manifestFile.open(QIODevice::WriteOnly);
    if (!success)
        throw NotebookException("Unabled to open file " % manifestFilename % " for writing");

    qint64 numWritten = manifestFile.write(serializedManifest.toUtf8());
    Q_UNUSED(numWritten);

    manifestFile.close();
}

QString FileNotebookFormat::getPageFilename(const QString& pageId) const
{
    QDir pagesDir = QDir(this->filename % "/pages");
    QString pageFilename = pagesDir.path() % '/' % pageId % ".html";
    return pageFilename;
}

QString FileNotebookFormat::serializeManifest() const
{
    QVariantList pagesList;
    QVariantList sectionsList;

    for (const QString& sectionName : this->notebook->getSectionNames())
    {
        QVariantList sectionPagesList;

        for (NotebookPage* page : this->notebook->getPages(sectionName))
        {
            QVariantMap pageMap;
            pageMap["id"] = page->getId();
            pageMap["name"] = page->getName();
            pagesList.append(pageMap);

            sectionPagesList.append(page->getId());
        }

        QVariantMap sectionMap;
        sectionMap["name"] = sectionName;
        sectionMap["pages"] = sectionPagesList;
        sectionsList.append(sectionMap);
    }

    QVariantMap top;
    top["pages"] = pagesList;
    top["sections"] = sectionsList;

    return QtJson::serialize(top);
}

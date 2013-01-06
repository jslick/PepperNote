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

FileNotebookFormat::FileNotebookFormat(const QString& filename, Notebook* parent) :
    NotebookFormat(parent),
    filename(filename)
{
}

void FileNotebookFormat::load()
{
    QString manifestJson = getFileUtf8(this->filename % "/manifest.json");
    this->manifest.parseJson(manifestJson);

    for (const QString& sectionName : this->manifest.getSectionNames())
    {
        typedef QPair<QString,QString> PageInfo;

        QList<PageInfo> pageInfos;
        this->manifest.getPages(sectionName, pageInfos);

        for (const PageInfo& pageInfo : pageInfos)
        {
            NotebookPage* page = new NotebookPage(pageInfo.first, pageInfo.second);
            this->notebook->addPage(sectionName, page);
        }
    }
}

QStringList FileNotebookFormat::getSectionNames() const
{
    return this->manifest.getSectionNames();
}

bool FileNotebookFormat::isPagePersisted(const QString& pageId) const
{
    QString pageFilename = this->getPageFilename(pageId);
    return QFile::exists(pageFilename);
}

QString FileNotebookFormat::getPageId(int sectionIndex, int pageIndex) const
{
    return this->manifest.getPageId(sectionIndex, pageIndex);
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

    // If the page is not yet persisted, add it to the manifest, and save the
    // manifest.
    if (!this->manifest.containsPage(page.getId()))
    {
        this->manifest.addPage(notebook.getPageSection(page), page.getId(), page.getName());
        this->saveManifest();
    }
    // Check to see if page name needs to be updated in manifest
    else if (this->manifest.getPageName(page.getId()) != page.getName())
    {
        this->manifest.setPageName(page.getId(), page.getName());
        this->saveManifest();
    }

    pageFile.close();
}

void FileNotebookFormat::movePage(NotebookPage& page, int places)
{
    this->manifest.movePage(page.getId(), places);
    this->saveManifest();
}

void FileNotebookFormat::movePageToSection(NotebookPage& page, const QString& sectionName)
{
    this->manifest.movePageToSection(page.getId(), sectionName);
    this->saveManifest();
}

void FileNotebookFormat::saveManifest()
{
    const QString serializedManifest = this->manifest.serialize();

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

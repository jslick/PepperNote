#include "filenotebookformat.h"
#include "utility.h"
#include "notebook.h"   // Notebook Exception
#include "notebookpage.h"

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
}

QStringList FileNotebookFormat::getSectionNames() const
{
    return this->manifest.getSectionNames();
}

QStringList FileNotebookFormat::getPageNames(const QString& sectionName) const
{
    return this->manifest.getPageNames(sectionName);
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

void FileNotebookFormat::savePage(NotebookPage& page, const QString& html)
{
    QDir notebookDir(this->filename);
    if (notebookDir.exists() == false)
        notebookDir.mkpath(".");

    QDir pagesDir = QDir(this->filename % "/pages");
    if (pagesDir.exists() == false)
        pagesDir.mkpath(".");

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
        this->manifest.addPage("General", page.getId());
        this->saveManifest();
    }

    pageFile.close();
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

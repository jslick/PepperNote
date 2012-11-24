#include "notebookformatmanifest.h"
#include "notebook.h"               // NotebookException
#include "qt-json/json.h"

#include <QStringList>

NotebookFormatManifest::NotebookFormatManifest(QObject* parent) :
    QObject(parent)
{
}

void NotebookFormatManifest::parseJson(const QString& manifestJson)
{
    bool ok;
    QVariantMap result = QtJson::parse(manifestJson, ok).toMap();
    if (!ok)
        throw NotebookException("Unable to parse Notebook manifest");

    // TODO:  Error-checking

    const QVariantList pagesList = result["pages"].toList();
    for (const QVariant& pageMapVariant : pagesList)
    {
        const QVariantMap pageMap = pageMapVariant.toMap();
        Page page;
        page.id = pageMap["id"].toString();
        page.name = pageMap["name"].toString();
        this->pages.append(page);
        this->pageIndex[page.id] = &this->pages.back();
    }

    const QVariantList sectionsList = result["sections"].toList();
    for (const QVariant& sectionMapVariant : sectionsList)
    {
        QVariantMap sectionMap = sectionMapVariant.toMap();
        Section section;
        section.name = sectionMap["name"].toString();

        const QStringList sectionPageList = sectionMap["pages"].toStringList();
        for (const QString& pageId : sectionPageList)
        {
            section.pages.append(this->pageIndex[pageId]);
        }

        this->sections.append(section);
        this->sectionIndex[section.name] = &this->sections.back();
    }
}

QString NotebookFormatManifest::serialize() const
{
    QVariantMap top;
    QVariantList pagesList = QVariantList();
    QVariantList sectionsList = QVariantList();

    for (const Page& page : this->pages)
    {
        QVariantMap pageMap;
        pageMap["id"] = page.id;
        pageMap["name"] = page.name;
        pagesList.append(pageMap);
    }

    for (const Section& section : this->sections)
    {
        QVariantList sectionPagesList;
        for (Page* page : section.pages)
        {
            sectionPagesList.append(page->id);
        }

        QVariantMap sectionMap;
        sectionMap["name"] = section.name;
        sectionMap["pages"] = sectionPagesList;
        sectionsList.append(sectionMap);
    }
    top["pages"] = pagesList;
    top["sections"] = sectionsList;

    return QtJson::serialize(top);
}

QString NotebookFormatManifest::getPageId(int sectionIndex, int pageIndex) const
{
    if (sectionIndex >= this->sections.length())
        throw NotebookException("Invalid section");
    if (pageIndex >= this->sections[sectionIndex].pages.length())
        throw NotebookException("Invalid page");

    Page* page = this->sections[sectionIndex].pages[pageIndex];
    Q_ASSERT_X(page, "NotebookFormatManifest::getPageId", "Null page stored in section");

    return page->id;
}

bool NotebookFormatManifest::containsPage(const QString& pageId) const
{
    return this->pageIndex[pageId] ? true : false;
}

void NotebookFormatManifest::addPage(const QString& sectionName, const QString& pageId)
{
    Page page;
    page.id = pageId;
    page.name = "Note";
    this->pages.append(page);
    this->pageIndex[pageId] = &this->pages.back();

    Section& section = this->findOrCreateSection(sectionName);
    section.pages.append(&this->pages.back());
}

NotebookFormatManifest::Section& NotebookFormatManifest::findOrCreateSection(const QString &sectionName)
{
    Section* section = this->sectionIndex[sectionName];
    if (!section)
    {
        Section newSection;
        newSection.name = sectionName;
        this->sections.append(newSection);
        section = &this->sections.back();

        this->sectionIndex[sectionName] = section;
    }
    return *section;
}

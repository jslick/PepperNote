/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "notebook.h"
#include "utility.h"
#include "notebookpage.h"
#include "notebookformat.h"
#include "notebookexception.h"

Notebook::Notebook(NotebookFormat* fileFormat, QObject* parent) :
    QObject(parent),
    fileFormat(fileFormat)
{
    if (fileFormat)
        fileFormat->setNotebook(*this);
}

Notebook::~Notebook()
{
    // Destroy pages
    for (Section& section : this->sections)
    {
        while (section.pages.isEmpty() == false)
        {
            NotebookPage* page = section.pages.takeLast();
            Q_ASSERT(page);
            delete page;
        }
    }

    if (this->fileFormat)
        delete this->fileFormat;
}

bool Notebook::isPagePersisted(const QString& pageId) const
{
    return this->fileFormat->isPagePersisted(pageId);
}

QStringList Notebook::getSectionNames() const
{
    QStringList sectionNames;

    for (const Section& section : this->sections)
    {
        sectionNames.append(section.name);
    }

    return sectionNames;
}

QList<NotebookPage*> Notebook::getPages(const QString& sectionName) const
{
    for (const Section& section : this->sections)
    {
        if (section.name == sectionName)
            return section.pages;
    }

    return QList<NotebookPage*>();
}

NotebookPage* Notebook::getFirstPage()
{
    if (this->sections.isEmpty())
        return 0;

    Section& firstSection = this->sections[0];
    return firstSection.pages.isEmpty() ? 0 : firstSection.pages[0];
}

QString Notebook::getPageContents(NotebookPage& page) const
{
    return this->isPagePersisted(page.getId()) ?
           this->getPageContents(page.getId()) :
           getFileUtf8(":/editor/html/new_page.html");
}

QString Notebook::getPageContents(const QString& pageId) const
{
    if (!this->fileFormat)
    {
        showMessage(tr("Cannot get page contents of unsaved notebook"));
        throw NotebookException("fileFormat is null");
    }

    return this->fileFormat->getPageContents(pageId);
}

void Notebook::addPage(const QString& sectionName, NotebookPage* page)
{
    Section& section = this->findOrCreateSection(sectionName);
    section.pages.append(page);

    emit pageAdded(sectionName, page);
}

void Notebook::savePage(NotebookPage& page, const QString& html)
{
    CHECK_POINTER_GUI(this->fileFormat, tr("Could not save the notebook:  The file format is unspecified"));

    try
    {
        this->fileFormat->savePage(*this, page, html);
    }
    catch (QtConcurrent::Exception& e)
    {
        Q_UNUSED(e);
        throw NotebookException("Could not save page");
    }
}

QString Notebook::getPageSection(const NotebookPage& page) const
{
    const Section* section = this->findSection(page);
    return section ? section->name : "";
}

void Notebook::movePage(NotebookPage& page, int places)
{
    for (Section& section : this->sections)
    {
        int index = section.pages.indexOf(&page);
        if (index >= 0)
        {
            int newIndex = index + places;
            if (newIndex < 0)
                newIndex = 0;
            section.pages.move(index, newIndex);

            this->fileFormat->movePage(page, places);

            emit pageMoved(&page, section.name, newIndex);
            return;
        }
    }
}

void Notebook::movePageToSection(NotebookPage& page, const QString& sectionName)
{
    Section* oldSection = this->findSection(page);
    Section* newSection = this->findSection(sectionName);
    if (!newSection)
        throw NotebookException("Could not page to section; section not found");

    if (oldSection)
        oldSection->pages.removeOne(&page);

    newSection->pages.append(&page);

    if (this->fileFormat->isPagePersisted(page.getId()))
        this->fileFormat->movePageToSection(page, sectionName);

    emit pageMoved(&page, sectionName, newSection->pages.length() - 1);
}

void Notebook::removePage(const QString& sectionName, NotebookPage& page)
{
    Section* section = this->findSection(sectionName);
    if (!section)
        throw NotebookException("Cannot remove page from section:  Section not found");

    section->pages.removeAll(&page);

    this->fileFormat->removePage(sectionName, page);

    emit pageRemoved(sectionName, &page);
}

Notebook::Section* Notebook::findSection(const QString& sectionName)
{
    for (Section& section: this->sections)
    {
        if (section.name == sectionName)
            return &section;
    }

    return 0;
}

Notebook::Section* Notebook::findSection(const NotebookPage& page)
{
    for (Section& section : this->sections)
    {
        if (section.pages.contains(const_cast<NotebookPage*>( &page )))
            return &section;
    }

    return 0;
}

const Notebook::Section* Notebook::findSection(const NotebookPage& page) const
{
    for (const Section& section : this->sections)
    {
        if (section.pages.contains(const_cast<NotebookPage*>( &page )))
            return &section;
    }

    return 0;
}

Notebook::Section& Notebook::findOrCreateSection(const QString& sectionName)
{
    Section* section = this->findSection(sectionName);
    if (section)
        return *section;

    Section newSection;
    newSection.name = sectionName;
    this->sections.append(newSection);
    return this->sections.back();
}

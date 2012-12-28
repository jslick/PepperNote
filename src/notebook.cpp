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
        this->fileFormat->savePage(page, html);
    }
    catch (QtConcurrent::Exception& e)
    {
        Q_UNUSED(e);

        throw NotebookException("Could not save page");
    }
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

            return;
        }
    }
}

Notebook::Section& Notebook::findOrCreateSection(const QString& sectionName)
{
    for (Section& section : this->sections)
    {
        if (section.name == sectionName)
            return section;
    }

    Section newSection;
    newSection.name = sectionName;
    this->sections.append(newSection);
    return this->sections.back();
}

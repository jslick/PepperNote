#include "notebook.h"
#include "utility.h"
#include "notebookpage.h"
#include "notebookformat.h"

Notebook::Notebook(NotebookFormat* fileFormat, QObject* parent) :
    QObject(parent),
    fileFormat(fileFormat)
{
    if (fileFormat)
        fileFormat->setNotebook(this);
}

Notebook::~Notebook()
{
    for (auto iter = this->loadedPagesByIndex.begin();
         iter != this->loadedPagesByIndex.end();
         iter++)
    {
        delete *iter;
    }

    if (this->fileFormat)
        delete this->fileFormat;
}

NotebookPage* Notebook::getFirstPage()
{
    NotebookPage* page = this->loadedPagesByIndex[0];
    if (!page)
    {
        if (!this->fileFormat)
        {
            showMessage(tr("Cannot get first page of unsaved notebook"));
            throw NotebookException("fileFormat is null");
        }

        const QString pageId = this->fileFormat->getPageId(0, 0);
        page = this->loadedPagesByIndex[0] = new NotebookPage(*this, pageId);
    }

    return page;
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

void Notebook::savePage(NotebookPage& page, const QString& html)
{
    CHECK_POINTER_GUI(this->fileFormat, tr("Could not save the notebook:  The file format is unspecified"));

    try
    {
        this->fileFormat->savePage(page, html);
    }
    catch (QtConcurrent::Exception& e)
    {
        // TODO:  Get message from exception; possibly make a base subclass
        //        of Exception for the whole application.
        throw NotebookException("Could not save page");
    }
}

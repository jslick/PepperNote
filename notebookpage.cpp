#include "notebookpage.h"
#include "utility.h"
#include "notebook.h"

#include <QFile>

NotebookPage::NotebookPage(Notebook& parent, const QString& pageId) :
    QObject(&parent),
    notebook(parent), pageId(pageId)
{
}

QString NotebookPage::getId() const
{
    return this->pageId;
}

QString NotebookPage::getHtml()
{
    return this->pageId.isEmpty() ?
           getFileUtf8(":/editor/html/new_note.html") :
           this->notebook.getPageContents(this->pageId);
}

void NotebookPage::saveContent(const QString& html)
{
    this->notebook.savePage(*this, html);
}

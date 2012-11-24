#include "notebookpage.h"
#include "utility.h"
#include "notebook.h"

#include <QUuid>
#include <QFile>

NotebookPage::NotebookPage(Notebook& parent, const QString& pageId) :
    QObject(&parent),
    notebook(parent), pageId(pageId.isEmpty() ? QUuid::createUuid().toString().mid(1, 36) : pageId)
{
}

bool NotebookPage::isPersisted() const
{
    return this->notebook.isPagePersisted(this->pageId);
}

QString NotebookPage::getId() const
{
    return this->pageId;
}

QString NotebookPage::getHtml()
{
    return this->notebook.getPageContents(this->pageId);
}

void NotebookPage::saveContent(const QString& html)
{
    this->notebook.savePage(*this, html);
}

#include "notebookpage.h"
#include "notebook.h"

const QString Notebook::FILE_EXTENSION = "notebook";

Notebook::Notebook(QObject* parent) :
    QObject(parent)
{
}

NotebookPage* Notebook::getFirstPage()
{
    // TODO:  Get first page from first section
    return new NotebookPage;
}

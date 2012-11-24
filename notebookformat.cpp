#include "notebookformat.h"
#include "notebook.h"

NotebookFormat::NotebookFormat(Notebook* parent) :
    QObject(parent),
    notebook(parent)
{
}

void NotebookFormat::setNotebook(Notebook& notebook)
{
    this->notebook = &notebook;
}

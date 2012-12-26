/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

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

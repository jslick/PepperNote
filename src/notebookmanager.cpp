/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "notebookmanager.h"
#include "notebook.h"
#include "notebookpage.h"
#include "filenotebookformat.h"

#include <QMutexLocker>
#include <QDebug>

NotebookManager NotebookManager::instance;

NotebookManager::NotebookManager(QObject* parent) :
    QObject(parent)
{
}

void NotebookManager::loadNotebook(const QString& filename)
{
    this->loadQueueMutex.lock();
    this->loadQueue.append(filename);
    this->loadQueueMutex.unlock();

    // TODO:  Load notebook from separate IO thread
    QMutexLocker locker(&this->loadQueueMutex);
    QString loadFilename = this->loadQueue.dequeue();

    NotebookFormat* fileFormat = new FileNotebookFormat(loadFilename);
    Notebook* notebook = new Notebook(fileFormat);
    fileFormat->load(); // load metadata

    emit notebookLoaded(notebook);
}

void NotebookManager::createNotebook(NotebookFormat* fileFormat)
{
    Notebook* notebook = new Notebook(fileFormat);
    NotebookPage* page = new NotebookPage("", tr("Note"));
    notebook->addPage("General", page);

    emit notebookLoaded(notebook);
}

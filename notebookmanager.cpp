#include "notebook.h"
#include "notebookmanager.h"

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
}

void NotebookManager::createNotebook()
{
    Notebook* notebook = new Notebook;

    emit notebookLoaded(notebook);
}

/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef NOTEBOOKMANAGER_H
#define NOTEBOOKMANAGER_H

#include <QObject>
#include <QQueue>
#include <QMutex>

class Notebook;
class NotebookFormat;

class NotebookManager : public QObject
{
    Q_OBJECT
public:
    static NotebookManager instance;    // Singleton; event-thread only access

    /**
     * @brief Load the notebook with the filename
     *
     * @param filename
     *
     * @note The NotebookManager is responsible for discoving the file format of
     *       a persisted Notebook.
     * @note Emits notebookLoaded
     */
    void loadNotebook(const QString& filename);

    /**
     * @brief Creates a notebook, and adds a default page to the notebook
     * @param fileFormat    The file format to save the notebook in
     *
     * @note Emits notebookLoaded
     */
    void createNotebook(NotebookFormat* fileFormat);

signals:

    /**
     * @param notebook
     */
    void notebookLoaded(Notebook* notebook);

public slots:

private:
    explicit NotebookManager(QObject* parent = 0);

    QQueue<QString> loadQueue;
    QMutex          loadQueueMutex;

};

#endif // NOTEBOOKMANAGER_H
